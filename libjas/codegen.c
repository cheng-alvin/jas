/**
 * MIT License
 * Copyright (c) 2023 Alvin Cheng (eventide1029@gmail.com)
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
#include "error.h"
#include "exe.h"
#include "label.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define CURR_TABLE instr_table[instr_arr[i].instr][j]

#define FREE_ALL(...)                                                   \
  do {                                                                  \
    void *pointers[] = {__VA_ARGS__};                                   \
    for (size_t i = 0; i < sizeof(pointers) / sizeof(*pointers); ++i) { \
      free(pointers[i]);                                                \
    }                                                                   \
  } while (0)

bool is_pre = false;

static buffer_t assemble(enum modes mode, instruction_t *instr_arr, size_t arr_size); // TODO Fix the stupid hack
buffer_t codegen(enum modes mode, instruction_t *instr_arr, size_t arr_size, enum codegen_modes exec_mode) {
  for (size_t i = 0; i < arr_size / sizeof(instruction_t); i++) {
    if (instr_arr[i].instr >= INSTR_DIR_LOCAL_LABEL) {
      if (instr_arr[i].operands[0].data)
        label_create(
            instr_arr[i].operands[0].data,
            instr_arr[i].instr == INSTR_DIR_GLOBAL_LABEL,
            instr_arr[i].instr == INSTR_DIR_GLOBAL_LABEL,
            0, i);
    }
  }

  is_pre = true;
  free(assemble(mode, instr_arr, arr_size).data);

  is_pre = false;
  const buffer_t code = assemble(mode, instr_arr, arr_size);

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
  const int base = 6 * 0x40;

  char shstrtab[] = "\0.shstrtab\0.strtab\0.symtab\0.text\0";
  buffer_t shstrtab_sect_head = exe_sect_header(1, 0x03, 0, base, sizeof(shstrtab));

  buf_write_byte(&strtab, 0);
  buf_write(&symtab, pad, 0x18);

  free(pad);

  for (size_t i = 0; i < label_table_size; i++) {
    // Refer to https://www.sco.com/developers/devspecs/gabi41.pdf - Figure 4-16
    uint8_t binding = 0;
    if (label_table[i].exported || label_table[i].ext) binding = 1;

    buf_write(&symtab, (uint32_t *)&strtab.len, 4);             // Name offset
    buf_write_byte(&symtab, (((binding) << 4) + ((0) & 0xf)));  // Info
    buf_write_byte(&symtab, 0);                                 // Other
    buf_write(&symtab, &(uint16_t){4}, 2);                      // Section index
    buf_write(&symtab, (uint64_t *)&label_table[i].address, 8); // Value
    buf_write(&symtab, &(uint64_t){0}, 8);                      // Size

    buf_write(&strtab, (uint8_t *)label_table[i].name, strlen(label_table[i].name) + 1);
  }

  buffer_t strtab_sect_head = exe_sect_header(11, 0x03, 0x2, base + sizeof(shstrtab), strtab.len);
  buffer_t symtab_sect_head = exe_sect_header(19, 0x02, 0x2, base + sizeof(shstrtab) + strtab.len, symtab.len);

  buffer_t text_sect_head = exe_sect_header(27, 0x01, 0x7, base + sizeof(shstrtab) + strtab.len + symtab.len, code.len);

  // Write and clean everything 🧹🧹

  label_destroy_all();
  buf_concat(&out, 4, &shstrtab_sect_head, &strtab_sect_head, &symtab_sect_head, &text_sect_head);
  FREE_ALL(shstrtab_sect_head.data, strtab_sect_head.data, symtab_sect_head.data, text_sect_head.data);

  buf_write(&out, (uint8_t *)shstrtab, sizeof(shstrtab));

  buf_concat(&out, 3, &strtab, &symtab, &code);
  FREE_ALL(strtab.data, symtab.data, code.data);

  return out;
}

static buffer_t assemble(enum modes mode, instruction_t *instr_arr, size_t arr_size) {
  arr_size /= sizeof(instruction_t);
  buffer_t buf = BUF_NULL;

  for (size_t i = 0; i < arr_size; i++) {
    if (instr_arr[i].instr > INSTR_SYSCALL) {
      if (instr_arr[i].instr == INSTR_DIR_WRT_BUF) {
        const buffer_t *data = (buffer_t *)instr_arr[i].operands[0].data;
        buf_write(&buf, data->data, data->len);
      }

      if (is_pre && instr_arr[i].instr <= INSTR_DIR_LOCAL_LABEL) {
        for (size_t j = 0; j < label_table_size; j++) {
          if (strcmp(label_table[j].name, instr_arr[i].operands[0].data) == 0) {
            label_table[j].address = buf.len;
            break;
          }
        }
      }

      continue;
    }

    instruction_t current = instr_arr[i];
    const enum operands operand_list[4] = {
        current.operands[0].type,
        current.operands[1].type,
        current.operands[2].type,
        current.operands[3].type,
    };

    enum enc_ident ident = op_ident_identify(operand_list);

    //! HARD CODED
    if (ident == OP_OI && instr_arr[i].instr != INSTR_MOV)
      ident = OP_MI;
    else if (ident == OP_MI && instr_arr[i].instr == INSTR_MOV)
      ident = OP_OI;

    instr_encode_table_t ref;
    unsigned int j = 0;
    while (CURR_TABLE.opcode_size != 0) {
      if (CURR_TABLE.ident == ident) {
        ref = CURR_TABLE;
        break;
      }
      j++;
    }

    if (ref.opcode_size == 0) {
      err("No corrsponding instruction opcode found.");
      free(buf.data);
      return BUF_NULL;
    }

    if (ref.pre != NULL) ref.pre(current.operands, &buf, &ref, (enum modes)mode);
    instr_encode_func(ident)(current.operands, &buf, &ref, (enum modes)mode);
  }

  return buf;
}
