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

static buffer_t assemble(enum modes mode, instruction_t *instr_arr, size_t arr_size,
                         instr_encode_table_t *tabs, bool is_pre, label_t *label_table, size_t label_table_size);
struct codegen_ret
codegen(enum modes mode, instruction_t **instr_input, size_t arr_count) {
  label_t *label_table = NULL;
  size_t label_table_size = 0;

  /* Implementing a "wrapper" due to old baggage ~~(And partially out of lazy-ness)~~ */
  const size_t arr_size = arr_count * sizeof(instruction_t);
  instruction_t *instr_arr = malloc(arr_size);

  for (size_t i = 0; i < arr_count; i++)
    instr_arr[i] = *instr_input[i];

  for (size_t i = 0; i < arr_size / sizeof(instruction_t); i++) {
    if (instr_arr[i].instr >= INSTR_DIR_LOCAL_LABEL) {
      if (instr_arr[i].operands[0].data)
        label_create(
            &label_table, &label_table_size,
            instr_arr[i].operands[0].data,
            instr_arr[i].instr == INSTR_DIR_GLOBAL_LABEL,
            instr_arr[i].instr == INSTR_DIR_GLOBAL_LABEL, //!!
            0);
    }
  }

  const instr_encode_table_t *tabs = get_instr_tabs(instr_arr, arr_size / sizeof(instruction_t));
  const uint8_t *pre_ret = assemble(mode, instr_arr, arr_size, tabs, true, label_table, label_table_size).data;
  if (pre_ret != NULL) free(pre_ret);

  const buffer_t code = assemble(mode, instr_arr, arr_size, tabs, false, label_table, label_table_size);
  FREE_ALL(tabs, instr_arr);

  const struct codegen_ret ret = {.code = code, .label_table = label_table, .label_table_size = label_table_size};
  return ret;
}

static buffer_t assemble(enum modes mode, instruction_t *instr_arr, size_t arr_size,
                         instr_encode_table_t *tabs, bool is_pre, label_t *label_table, size_t label_table_size) {

  arr_size /= sizeof(instruction_t);
  buffer_t buf = BUF_NULL;
  size_t label_index = 0;

  for (size_t i = 0; i < arr_size; i++) {
    /* -- Sanity checks -- */
    if (is_pre && label_table_size == 0) break;
    if (is_pre && label_index >= label_table_size) break;
    if (instr_arr[i].operands == NULL) continue;

    if (INSTR_DIRECTIVE(instr_arr[i].instr)) {
      if (instr_arr[i].instr == INSTR_DIR_WRT_BUF) {
        const buffer_t *data = (buffer_t *)instr_arr[i].operands[0].data;
        buf_write(&buf, data->data, data->len);
      }
      if (is_pre && IS_LABEL(instr_arr[i])) {
        for (size_t j = 0; j < label_table_size; j++) {
          label_t *tab = label_table;
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

    uint8_t opcode_sz = ref.opcode_size;
    if (ref.byte_opcode_size > 0) opcode_sz = ref.byte_opcode_size;

    instruction_t current = instr_arr[i];

    for (uint8_t j = 1; j < 4; j++) {
      if (current.operands[j].type == OP_NULL) break;

      bool valid_operands =
          op_sizeof(current.operands[j].type) >=
          op_sizeof(current.operands[j - 1].type);

      if (valid_operands) continue;

      err("invalid operand type");
      return BUF_NULL;
    }

    op_write_prefix(&buf, current.operands, mode);
    buf_write(&buf, op_write_opcode(current.operands, &ref), opcode_sz);
    const encoder_t function_ptr = enc_lookup(ref.ident);

    if (function_ptr == NULL) {
      if (instr_arr[i].operands[0].type != OP_NULL)
        err("instruction unsupported or improper usage");
      continue;
    }
    function_ptr(current.operands, &buf, &ref, mode, label_table, label_table_size);
  }

  return buf;
}

buffer_t assemble_instr(enum modes mode, instruction_t *instr) {
  return codegen(mode, &instr, 1).code;
}