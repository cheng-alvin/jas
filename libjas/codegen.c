/**
 * MIT License
 * Copyright (c) 2023 Alvin Cheng <eventide1029@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @see `LICENSE`
 */

#include "codegen.h"
#include "encoder.h"
#include "error.h"
#include "exe.h"
#include "label.h"
#include "operand.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define FREE_ALL(...)                                                   \
  do {                                                                  \
    void *pointers[] = {__VA_ARGS__};                                   \
    for (size_t i = 0; i < sizeof(pointers) / sizeof(*pointers); ++i) { \
      free(pointers[i]);                                                \
    }                                                                   \
  } while (0)

static instr_encode_table_t *get_instr_tabs(instruction_t *instr_arr, size_t arr_size) {
  instr_encode_table_t *tabs = malloc(sizeof(instr_encode_table_t) * arr_size);
  for (size_t i = 0; i < arr_size; i++) {
    if (IS_LABEL(instr_arr[i])) {
      tabs[i] = INSTR_TAB_NULL;
      continue;
    }
    tabs[i] = instr_get_tab(instr_arr[i]);
  }
  return tabs;
}

static buffer_t assemble(enum modes mode, instruction_t *instr_arr, size_t arr_size, instr_encode_table_t *tab, bool is_pre);
buffer_t codegen(enum modes mode, instruction_t **instr_input, size_t arr_count, enum codegen_output exec_mode) {
  /* Implementing a "wrapper" due to old baggage ~~(And partially out of lazy-ness)~~ */
  const size_t arr_size = arr_count * sizeof(instruction_t);
  instruction_t *instr_arr = malloc(arr_size);

  for (size_t i = 0; i < arr_count; i++)
    instr_arr[i] = *instr_input[i];

  for (size_t i = 0; i < arr_size / sizeof(instruction_t); i++) {
    if (instr_arr[i].instr >= INSTR_DIR_LOCAL_LABEL) {
      if (instr_arr[i].operands[0].data)
        label_create(
            instr_arr[i].operands[0].data,
            instr_arr[i].instr == INSTR_DIR_GLOBAL_LABEL,
            instr_arr[i].instr == INSTR_DIR_GLOBAL_LABEL,
            0);
    }
  }

  const instr_encode_table_t *tabs = get_instr_tabs(instr_arr, arr_size / sizeof(instruction_t));
  const uint8_t *pre_ret = assemble(mode, instr_arr, arr_size, tabs, true).data;
  if (pre_ret != NULL) free(pre_ret);

  const buffer_t code = assemble(mode, instr_arr, arr_size, tabs, false);
  free(tabs);
  free(instr_arr);

  if (exec_mode == CODEGEN_RAW) return code;

  if (mode != MODE_LONG) {
    err("Only 64-bit ELF formats supported for object code generation.");
    return BUF_NULL;
  }

  buffer_t strtab = BUF_NULL;
  buffer_t symtab = BUF_NULL;
  buffer_t out = BUF_NULL;

  buffer_t header = exe_header(0x40, 5, 1);
  buf_concat(&out, 1, &header);
  free(header.data);

  const uint8_t *pad = calloc(0x40, 1);
  buf_write(&out, pad, 0x40); // Padding

  /**
   * @note
   * The file offset of the section header table as seen with the expressions
   * such as `6 * 0x40 + sizeof(shstrtab)` etc is shown as `6 * 0x40` because
   * the ELF header is 64 bytes long and the section header table is 64 bytes
   * EACH, given that there are 5 sections, the total size of the section header
   * table is 256 bytes, hence the `6 * 0x40` expression.
   *
   * Also, the data offset must also be taken into account when calculating the
   * file offset of the section header table. as we need some space for the data
   * itself for the section.
   */
  size_t base = 6 * 0x40;

  char shstrtab[] = "\0.shstrtab\0.strtab\0.symtab\0.text\0";
  buffer_t shstrtab_sect_head = exe_sect_header(1, 0x03, 0, &base, sizeof(shstrtab));

  buf_write_byte(&strtab, 0);
  buf_write(&symtab, pad, 0x18);

  // Writing section name to symbol table
  // For some reason the gcc compiler does not link if there's no filename.

  const buffer_t section_ent = exe_sym_ent(".text", 0x0, 4, &strtab, (((1) << 4) + ((3) & 0xf)));
  buf_concat(&symtab, 1, &section_ent);
  free(section_ent.data);
  free(pad);

  const size_t label_table_size = label_get_size();
  const label_t *label_table = label_get_table();

  for (size_t i = 0; i < label_table_size; i++) {
    // Refer to https://www.sco.com/developers/devspecs/gabi41.pdf - Figure 4-16
    uint8_t binding = 0;
    if (label_table[i].exported || label_table[i].ext) binding = 1;

    const buffer_t ent =
        exe_sym_ent(label_table[i].name, label_table[i].address, 4, &strtab,
                    (((binding) << 4) + ((0) & 0xf)));

    buf_concat(&symtab, 1, &ent);
    free(ent.data);
  }

  buffer_t strtab_sect_head = exe_sect_header(11, 0x03, 0x2, &base, strtab.len);
  buffer_t symtab_sect_head = exe_sect_header(19, 0x02, 0x2, &base, symtab.len);
  buffer_t text_sect_head = exe_sect_header(27, 0x01, 0x7, &base, code.len);

  // Write and clean everything 🧹🧹

  label_destroy_all();
  buf_concat(&out, 4, &shstrtab_sect_head, &strtab_sect_head, &symtab_sect_head, &text_sect_head);
  FREE_ALL(shstrtab_sect_head.data, strtab_sect_head.data, symtab_sect_head.data, text_sect_head.data);

  buf_write(&out, (uint8_t *)shstrtab, sizeof(shstrtab));

  buf_concat(&out, 3, &strtab, &symtab, &code);
  FREE_ALL(strtab.data, symtab.data, code.data);

  return out;
}

static buffer_t assemble(enum modes mode, instruction_t *instr_arr, size_t arr_size, instr_encode_table_t *tabs, bool is_pre) {
  arr_size /= sizeof(instruction_t);
  buffer_t buf = BUF_NULL;
  size_t label_index = 0;

  for (size_t i = 0; i < arr_size; i++) {
    /* -- Sanity checks -- */
    if (is_pre && label_get_size() == 0) break;
    if (is_pre && label_index >= label_get_size()) break;
    if (instr_arr[i].operands == NULL) continue;

    if (INSTR_DIRECTIVE(instr_arr[i].instr)) {
      if (instr_arr[i].instr == INSTR_DIR_WRT_BUF) {
        const buffer_t *data = (buffer_t *)instr_arr[i].operands[0].data;
        buf_write(&buf, data->data, data->len);
      }
      if (is_pre && IS_LABEL(instr_arr[i])) {
        for (size_t j = 0; j < label_get_size(); j++) {
          label_t *tab = label_get_table();
          if (strcmp(tab[j].name, instr_arr[i].operands[0].data) == 0) {
            tab[j].address = buf.len;
            break;
          }
        }
        label_index++;
      }

      continue;
    }

    const instr_encode_table_t ref = tabs[i];
    instruction_t current = instr_arr[i];
    if (ref.pre != NULL) ref.pre(current.operands, &buf, &ref, mode);
    enc_lookup(ref.ident)(current.operands, &buf, &ref, mode);
  }

  return buf;
}

buffer_t assemble_instr(enum modes mode, instruction_t *instr) {
  return codegen(mode, &instr, 1, CODEGEN_RAW);
}