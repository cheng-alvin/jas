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
#include <limits.h>
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

  for (uint8_t i = 0; i < tab.operand_count; i++) {
    enum enc_ident option = tab.operand_descriptors[i].encoder;
    if (option == ENC_IGNORE) continue;

    uint8_t operand_size = op_sizeof(instr.operands[i].type);

    if (op_rel(instr.operands[i].type)) continue;
    if (op_imm(instr.operands[i].type)) {
      if (option == ENC_OPCODE_APPENDED) {
        serialized->opcode[serialized->opcode_size - 1] +=
            (uint8_t)instr.operands[i].imm;

        continue;
      }
      serialized->imm = instr.operands[i].imm;
      serialized->imm_size = operand_size;
      continue;
    }

    uint8_t reg = reg_lookup_val(&instr.operands[i].mem.reg);
    serialized->has_modrm = true;

    if (op_r(instr.operands[i].type)) {
      if ((uint8_t)option >= 8) serialized->modrm.rm = reg;
      else {
        serialized->modrm = (struct op_modrm) \ 
            {OP_MODRM_MODE_REG, (uint8_t)option, reg};
      }

      continue;
    }
    serialized->modrm.rm = reg;

    // Assumption that all operands are memory from this point
    // onwards, generates ModR/M and SIB bytes where applicable.

    enum registers index = instr.operands[i].mem.reg_disp;
    if (index == REG_NULL) err("an index is required.");

    uint64_t disp = instr.operands[i].mem.disp;
    serialized->disp = disp;

    uint8_t disp_sz;
    serialized->modrm.mod = op_modrm_mode(disp, &disp_sz);
    serialized->disp_size = disp_sz;

    if (instr.operands[i].mem.reg_disp != REG_NULL || reg == 4) {
      uint8_t index_val = reg_lookup_val(index);
      if (index_val == REG_NULL) index_val = reg;

      /// @note where the SIB byte is used, the R/M field of
      /// ModR/M must be manually overriden to `0b100` (4).
      serialized->modrm.rm = 4;

      serialized->has_sib = true;
      serialized->sib = (op_sib_t) \ 
        {instr.operands[i].mem.scale, index_val, reg};
    }
  }

  return serialized;
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