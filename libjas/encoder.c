/**
 * MIT License
 * Copyright (c) 2023-2025 Alvin Cheng <eventide1029@gmail.com>
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

#include "encoder.h"
#include "error.h"
#include "instruction.h"
#include <stdbool.h>
#include <stdlib.h>

struct enc_serialized_instr *enc_serialize(instr_generic_t *input, enum modes mode) {
  if (input->type != INSTR) return NULL;
  const instruction_t instr = input->instr;
  struct instr_encode_table tab = instr_get_tab(instr);

  struct enc_serialized_instr *serialized =
      calloc(1, sizeof(enc_serialized_instr_t));

  // clang-format off
  op_write_prefix(&serialized->prefixes, 
    instr.operands, mode, &serialized->rex);
  // clang-format on

  memcpy(serialized->opcode, tab.opcode, tab.opcode_size);
  serialized->opcode_size = tab.opcode_size;

  if (!enc_operands_valid(tab, instr)) {
    err("operand type mismatch");
    free(serialized);

    return NULL;
  }
}

static bool enc_operands_valid(struct instr_encode_table tab, instruction_t instr) {
  uint8_t i = 0;
  for (; (uint8_t)instr.operands[i].type; i++)
    ;

  if (tab.operand_count != i + 1) return false;
  for (uint8_t j = 0; j < tab.operand_count; j++) {
    enum operands op = instr.operands[j].type;
    if (tab.operand_descriptors[j].type != op) return false;
  }

  return true;
}