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

#include "rex.h"

rex_t rex_apply(instruction_t *input) {
  rex_t rex = REX_DEFAULT;
  instr_encode_table_t tab = (instr_encode_table_t){0};

  for (uint8_t i = 0; i < 4; i++) {
    const operand_t op = input->operands[i];
    if (op.type == OP_NULL) break;

    if (op_sizeof(op.type) == 64) rex |= REX_W;

    /// @note such usage of REX fields is only applicable
    /// in context of SIB-based memory operands and register
    /// usage!
    if (op_rm(op.type) && op.mem.src_type == SIB) {
      if (!tab.opcode_size) tab = instr_get_tab(*input);

      const enum enc_ident option =
          tab.operand_descriptors[i].encoder;

      if (option == ENC_DEFAULT) rex |= REX_R;
      if (reg_needs_rex(op.mem.src.sib.reg_disp)) rex |= REX_X;
      else
        rex |= REX_B;
    }
  }

  return rex;
}