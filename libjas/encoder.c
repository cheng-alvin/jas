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

static bool op_valid(instr_encode_table_t tab, instruction_t instr);
struct enc_serialized_instr *enc_serialize(instr_generic_t *input, enum modes mode) {
  if (input->type != INSTR) return NULL;

  const instruction_t instr = input->instr;
  const struct instr_encode_table tab = instr_get_tab(instr);

  if (!enc_operands_valid(tab, instr)) {
    err("operand type mismatch");
    return NULL;
  }

  struct enc_serialized_instr *serialized =
      calloc(1, sizeof(enc_serialized_instr_t));

  op_write_prefix(&serialized->prefixes, instr.operands, mode);
  serialized->rex = rex_apply(&instr);

  memcpy(serialized->opcode, tab.opcode, tab.opcode_size);
  serialized->opcode_size = tab.opcode_size;

  for (uint8_t i = 0; i < tab.operand_count; i++) {
    op_mem_src_t mem_src = instr.operands[i].mem.src;
    enum enc_ident option = tab.operand_descriptors[i].encoder;

    if (option == ENC_IGNORE) continue;
    uint8_t operand_size = op_sizeof(instr.operands[i].type);

    if (op_rel(instr.operands[i].type)) continue;
    if (op_imm(instr.operands[i].type)) {
      serialized->imm = instr.operands[i].imm;
      serialized->imm_size = operand_size;
      continue;
    }

    uint8_t reg = reg_lookup_val(&(mem_src.sib.reg));
    serialized->has_modrm = true;

    if (op_r(instr.operands[i].type)) {
      if (option == ENC_DEFAULT) serialized->modrm.reg = reg;

      /// @note such option depicts Intel's /r option, using
      /// the register raw encoded value as the register field
      /// of the ModR/M byte.

      if (option == ENC_RM) {
        serialized->modrm.rm = reg; // Assign directly to R/M field
        serialized->modrm.mod = OP_MODRM_MODE_REG;
      } else if ((uint8_t)option < 8) {
        serialized->modrm = (struct op_modrm) \ 
            {OP_MODRM_MODE_REG, (uint8_t)option, reg};
      }

      if (option == ENC_OPCODE_APPENDED) {
        serialized->opcode[serialized->opcode_size - 1] +=
            reg_lookup_val(&(mem_src.sib.reg));
      }

      continue;
    }
    serialized->modrm.rm = reg;

    // Assumption that all operands are memory from this point
    // onwards, generates ModR/M and SIB bytes where applicable.

    uint64_t disp = instr.operands[i].mem.disp;
    if (instr.operands[i].mem.src_type == LABEL) {
      serialized->disp += disp;
      continue;
    }

    enum registers index = mem_src.sib.reg_disp;
    if (index == REG_NULL) err("an index is required.");

    serialized->disp = disp;

    uint8_t disp_sz;
    serialized->modrm.mod = op_modrm_mode(disp, &disp_sz);
    serialized->disp_size = disp_sz;

    if (mem_src.sib.reg_disp != REG_NULL || reg == 4) {
      uint8_t index_val = reg_lookup_val(index);
      if (index == REG_NULL) index_val = reg;

      /// @note where the SIB byte is used, the R/M field of
      /// ModR/M must be manually overriden to `0b100` (4).
      serialized->modrm.rm = 4;

      serialized->has_sib = true;
      serialized->sib = (op_sib_t) \ 
        {mem_src.sib.scale, index_val, reg};
    }
  }

  return serialized;
}

#define write_imm_data(data, size) \ 
  buf_write(&buf, endian(&data), size);

buffer_t enc_deserialize(enc_serialized_instr_t *in, buffer_t buf) {
  buf_concat(&buf, 1, &(in->prefixes));
  if (in->rex != REX_DEFAULT) buf_write_byte(&buf, in->rex);

  buf_write(&buf, in->opcode, in->opcode_size);

  /// @note Structures that are packed cannot be written
  /// directly due to inherent compiler protection measures.

  if (in->has_modrm) {
    buf_write(&buf, (uint8_t *)&in->modrm, 1);
    if (in->has_sib) buf_write(&buf, (uint8_t *)&in->sib, 1);
  }

  write_imm_data(in->imm, in->imm_size);
  write_imm_data(in->disp, in->disp_size);

  return buf;
}
#undef write_imm_data // Macro no longer applicable.

static bool op_valid(instr_encode_table_t tab, instruction_t instr) {
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