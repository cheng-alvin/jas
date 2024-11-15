/**
 * MIT License
 * Copyright (c) 2023-2024 Alvin Cheng (eventide1029@gmail.com)
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
#include "buffer.h"
#include "endian.h"
#include "error.h"
#include "instruction.h"
#include "label.h"
#include "mode.h"
#include "register.h"
#include <stdlib.h>

#define OP_OPCODE_HELPER (op_sizeof(op_arr[0].type) == 8 ? instr_ref->byte_instr_opcode : instr_ref->opcode)
#define EMPTY_SIB 0x24

static void ref_label(operand_t *op_arr, buffer_t *buf, uint8_t index) {
  const uint8_t rel_sz = op_sizeof(op_arr[index].type) / 8;

  label_t *label = label_lookup((char *)op_arr[index].label);
  if (!label) {
    for (size_t i = 0; i < label_table_size; i++) {
      if (label_table[i].ext && label_table[i].address == 0 && label_table[i].instr_index == 0) {
        label = &label_table[i];
        break;
      }
    }
  }

  if (!label) {
    err("Referenced label was not found.");
    return;
  }

  if (label->ext)
    label_create(op_arr[index].label, false, true, buf->len, 0);

  ptrdiff_t rel_offset = label->address - (buf->len + rel_sz - 1);
  buf_write(buf, label->ext ? &(uint64_t){0} : (uint8_t *)&rel_offset, rel_sz);
}

void d(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  /**
   * @brief This opcode identity should:
   *   1. Write the opcode to the buffer ✅
   *   2. Calculate the relative offset of the label ✅
   *   3. Write the relative offset to the buffer - kinda ✅
   */

  // IN BYTES!!!
  const uint8_t rel_sz = op_sizeof(op_arr[0].type) / 8;

  if (rel_sz == 8) {
    err("A relative address cannot be 64 bit-sized.");
    return;
  }

  check_mode(mode, instr_ref->support);
  buf_write(buf, OP_OPCODE_HELPER, instr_ref->opcode_size);

  // Calculate the relative offset of the label
  ref_label(op_arr, buf, 0);
}

void i(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  //  Error checking - A register only & only 8, 16, 32 bit-sized operands
  if (reg_lookup_val(op_arr[0].data) != 0 && !reg_needs_rex(op_arr[0].data)) {
    err("Instruction identity must use an \"A\" register!");
    return;
  }

  if (op_arr[1].type == OP_IMM64) {
    err("Invalid immediate value.");
    return;
  }

  buf_write(buf, OP_OPCODE_HELPER, instr_ref->opcode_size);

  const uint8_t imm_size = op_sizeof(op_arr[1].type) / 8;
  uint8_t *imm = (uint8_t *)op_arr[1].data;

  buf_write(buf, imm, imm_size);
}

void m(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  const uint8_t opcode_extend = instr_ref->opcode_ext << 3;
  const uint8_t rm = reg_lookup_val(op_arr[0].data);

  op_write_prefix(buf, op_arr, mode);
  check_mode(mode, instr_ref->support);

  buf_write(buf, OP_OPCODE_HELPER, instr_ref->opcode_size);
  buf_write_byte(buf, op_modrm_mode(op_arr[0]) | opcode_extend | rm);

  if (op_m(op_arr[0].type))
    rm == 4 ? buf_write_byte(buf, EMPTY_SIB) : NULL;

  if (op_arr[0].offset != 0)
    buf_write(buf, (uint8_t *)&op_arr[0].offset, 4);
}

void mi(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  m(op_arr, buf, instr_ref, mode);
  if (op_arr[1].type == OP_IMM64) {
    err("Invalid immediate value.");
    return;
  }

  const uint8_t imm_size = op_sizeof(op_arr[1].type) / 8;
  uint8_t *imm = (uint8_t *)op_arr[1].data;
  buf_write(buf, imm, imm_size);
}

void mr(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  const uint8_t reg = reg_lookup_val(op_arr[1].data);
  const uint8_t rm = reg_lookup_val(op_arr[0].data);
  op_write_prefix(buf, op_arr, mode);

  check_mode(mode, instr_ref->support);

  buf_write(buf, OP_OPCODE_HELPER, instr_ref->opcode_size);

  buf_write_byte(buf, op_modrm_mode(op_arr[0]) | reg << 3 | rm);

  if (op_m(op_arr[0].type))
    rm == 4 ? buf_write_byte(buf, EMPTY_SIB) : NULL;

  if (op_arr[0].offset != 0)
    buf_write(buf, (uint8_t *)&op_arr[0].offset, 4);
}

void o(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  const uint8_t reg = reg_lookup_val(op_arr[0].data);
  op_write_prefix(buf, op_arr, mode);

  check_mode(mode, instr_ref->support);

  uint8_t *data = &(uint8_t){*(OP_OPCODE_HELPER) + (uint8_t)reg};
  buf_write(buf, data, instr_ref->opcode_size);
}

void oi(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  o(op_arr, buf, instr_ref, mode);

  const uint8_t imm_size = op_sizeof(op_arr[1].type) / 8;
  uint8_t *imm = (uint8_t *)op_arr[1].data;
  buf_write(buf, imm, imm_size);
}

void rm(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  const uint8_t reg = reg_lookup_val(op_arr[0].data);
  const uint8_t rm = reg_lookup_val(op_arr[1].data);
  op_write_prefix(buf, op_arr, mode);

  check_mode(mode, instr_ref->support);
  buf_write(buf, OP_OPCODE_HELPER, instr_ref->opcode_size);

  buf_write_byte(buf, op_modrm_mode(op_arr[1]) | reg << 3 | rm);

  if (op_m(op_arr[1].type))
    rm == 4 ? buf_write_byte(buf, EMPTY_SIB) : NULL;

  if (op_arr[1].offset != 0)
    buf_write(buf, (uint8_t *)&op_arr[1].offset, 4);
}

void zo(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  check_mode(mode, instr_ref->support);
  buf_write(buf, instr_ref->opcode, instr_ref->opcode_size);
}