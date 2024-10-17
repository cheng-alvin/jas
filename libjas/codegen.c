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
#include "buffer.h"
#include "error.h"
#include "instruction.h"
#include "label.h"
#include "mode.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define CURR_TABLE instr_table[instr_arr[i].instr][j]

static buffer_t assemble(enum modes mode, instruction_t *instr_arr, size_t arr_size, bool pre); // TODO Fix the stupid hack
buffer_t codegen(enum modes mode, instruction_t *instr_arr, size_t arr_size) {
  assemble(mode, instr_arr, arr_size, true);
  return assemble(mode, instr_arr, arr_size, false);
}

static buffer_t assemble(enum modes mode, instruction_t *instr_arr, size_t arr_size, bool pre) {
  arr_size /= sizeof(instruction_t);
  buffer_t buf = BUF_NULL;

  for (size_t i = 0; i < arr_size; i++) {
    if (instr_arr[i].instr == NULL && instr_arr[i].operands == NULL) {
      if (!pre) continue;
      for (size_t k = 0; k < label_table_size; k++) {
        if (label_table[k].instr_index == i)
          label_table[k].address = buf.len - 1;
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

    enum op_ident ident = op_ident_identify(operand_list);

    instr_encode_table_t ref;
    unsigned int j = 0;
    while (CURR_TABLE.opcode_size != NULL) {
      if (CURR_TABLE.ident == ident) {
        ref = CURR_TABLE;
        break;
      }
      j++;
    }

    if (ref.opcode_size == NULL) {
      err("No corrsponding instruction opcode found.");
      free(buf.data);
      return BUF_NULL;
    }

    if (ref.pre != NULL) ref.pre(current.operands, &buf, &ref, (enum modes)mode);
    instr_encode_func(ident)(current.operands, &buf, &ref, (enum modes)mode);
  }

  return buf;
}
