/**
 * MIT License
 * Copyright (c) 2023-2026 Alvin Cheng <eventide1029@gmail.com>
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
#include "instruction.h"

rex_t rex_apply(instruction_t *input, instr_encode_table_t *input_tab) {
  rex_t rex = REX_DEFAULT;
  instr_encode_table_t tab = (instr_encode_table_t){0};
  if (input_tab) tab = *input_tab;

  for (uint8_t i = 0; i < 4; i++) {
    const operand_t op = input->operands[i]; // Preserves current operand

    if (op_sizeof(op.type) == 64) rex |= REX_W;
    if (op.type == OP_NULL || !op_rm(op.type)) break;

    /// @note Boolean value representing whether the primary
    /// register field of the current operand requires the use
    /// of the register extension prefix.
    const bool primary_reg = reg_needs_rex(op.mem.src.sib.reg);

    if (!op.mem.src.sib.reg_disp) { // Checks whether is SIB-addressed.
      enum enc_ident encoder = tab.operand_descriptors[i].encoder;

      // clang-format off
      switch (encoder) {
        case ENC_RM: case ENC_OPCODE_APPENDED:
          if (primary_reg) rex |= REX_B;
          break;

        default: if (primary_reg) rex |= REX_R; break;
      }
      // clang-format on
    } else {
      bool secondary_reg = reg_needs_rex(op.mem.src.sib.reg_disp);

      if (primary_reg) rex |= REX_R;
      if (secondary_reg) rex |= REX_X;
    }
  }
  return rex;
}