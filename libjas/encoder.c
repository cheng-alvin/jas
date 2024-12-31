/**
 * MIT License
 * Copyright (c) 2023-2024 Alvin Cheng <eventide1029@gmail.com>
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
#include "label.h"
#include "register.h"
#include <stdlib.h>

#define OP_OPCODE_HELPER (op_sizeof(op_arr[0].type) == 8 ? instr_ref->byte_instr_opcode : instr_ref->opcode)
#define EMPTY_SIB 0x24

/**
 * @brief
 * - A label is a memory address NOT a value!
 * therefore, a instruction like mov rax, label
 * should move the ADDRESS of the label to rax
 * NOT THE VALUE
 *
 * - A empty 0x25 SIB byte is written if we want a
 * base pointer register (rsp, sp or esp) to be used
 * with an absolute address.
 */
static void ref_label(operand_t *op_arr, buffer_t *buf, uint8_t index) {
  const uint8_t rel_sz = op_sizeof(op_arr[index].type) / 8;

  label_t *label = label_lookup((char *)op_arr[index].label);
  if (!label) {
    err("Referenced label was not found.");
    return;
  }

  extern bool is_pre;
  int32_t rel_offset = is_pre ? 0 : label->address - (buf->len + rel_sz - 1) - 1;
  buf_write(buf, (uint8_t *)&rel_offset, rel_sz);
}

static void write_offset(uint8_t mode, buffer_t *buf, operand_t *op_arr, uint8_t index) {
  switch (mode) {
  case OP_MODRM_DISP8:
    buf_write_byte(buf, (int8_t)op_arr[index].offset);
    break;

  case OP_MODRM_DISP32:
    buf_write(buf, (uint8_t *)&(int32_t){op_arr[index].offset}, 4);
    break;
  }
}

DEFINE_ENCODER(i) {
  //  Error checking - A register only & only 8, 16, 32 bit-sized operands
  if (reg_lookup_val(op_arr[0].data) != 0 && !reg_needs_rex((enum registers)op_arr[0].data)) {
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

DEFINE_ENCODER(m) {
  const uint8_t opcode_extend = instr_ref->opcode_ext << 3;
  const uint8_t rm = reg_lookup_val(op_arr[0].data);

  op_write_prefix(buf, op_arr, mode);
  check_mode(mode, instr_ref->support);
  buf_write(buf, OP_OPCODE_HELPER, instr_ref->opcode_size);

  const uint8_t mod = op_modrm_mode(op_arr[0]);
  buf_write_byte(buf, mod | opcode_extend | rm);

  if (op_m(op_arr[0].type) && rm == 4)
    buf_write_byte(buf, EMPTY_SIB);

  const enum registers deref_reg = (*(enum registers *)op_arr[0].data);
  if (deref_reg == REG_RIP || deref_reg == REG_EIP || deref_reg == REG_IP)
    ref_label(op_arr, buf, 0);

  if (rm == 5 && op_arr[0].offset == 0)
    buf_write_byte(buf, 0);

  write_offset(mod, buf, op_arr, 0);
}

/**
 * @note - Internal documentation (31th Dec 2024 - Last day of '24 🎉) 
 * This function will be used to write a number (aka a immediate value)
 * to the buffer, which is considered a common ground for many encoder
 * identities, such as `mi`, `oi` etc that also require a immediate value
 * to be written to the buffer for the encoding.
 */
static void i_common(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  const uint8_t imm_size = op_sizeof(op_arr[1].type) / 8;
  uint8_t *imm = (uint8_t *)op_arr[1].data;
  buf_write(buf, imm, imm_size);
}

DEFINE_ENCODER(d) {
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

DEFINE_ENCODER(mi) {
  m(op_arr, buf, instr_ref, mode);
  i_common(op_arr, buf, instr_ref, mode);

  if (op_arr[1].type == OP_IMM64) {
    err("Invalid immediate value.");
    return;
  }
}

static void rm_mr_common(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode, enum enc_ident ident) {
  /**
   * @brief This opcode identity is a "Common ground for MR and RM"
   * Since rm and mr just has to be flipped, we can just use a boolean
   * to determine which one to use, and then reference it.
   *
   * @note An empty SIB byte will be written if the register is 4
   * (A rsp, sp or esp) register that activates the SIB byte.
   */

  const bool is_rm = ident == ENC_RM;

  // Register - Since this is accessed quite often
  register const uint8_t reg_idx = is_rm ? 0 : 1;
  register const uint8_t rm_idx = is_rm ? 1 : 0;

  const uint8_t reg = reg_lookup_val(op_arr[reg_idx].data);
  const uint8_t rm = reg_lookup_val(op_arr[rm_idx].data);

  op_write_prefix(buf, op_arr, mode);

  check_mode(mode, instr_ref->support);
  buf_write(buf, OP_OPCODE_HELPER, instr_ref->opcode_size);

  const uint8_t mod = op_modrm_mode(op_arr[rm_idx]);
  buf_write_byte(buf, mod | (reg << 3) | rm);

  /**
   * @note
   * Some edge cases for the ModR/M byte:
   *
   * 1. If the register is RIP, EIP or IP, then the label will be referenced
   * through the relative offset via the `ref_label` function, see above.
   *
   * 2. If the register is a 4 (rsp, sp or esp), then an empty SIB byte will
   * be written to the buffer. Since the value 4 in the reg field will auto
   * matically trigger the SIB to be expected.
   *
   * 3. If the register rbp is referenced, then the offset will be written
   * as 0 and the mode will be set to a 8 bit displacement.
   */

  const enum registers deref_reg = (*(enum registers *)op_arr[rm_idx].data);
  if (deref_reg == REG_RIP || deref_reg == REG_EIP || deref_reg == REG_IP) {
    op_arr[rm_idx].type = deref_reg == REG_RIP ? OP_M32 : op_arr[rm_idx].type;
    ref_label(op_arr, buf, rm_idx);
    op_arr[rm_idx].type = deref_reg == REG_RIP ? OP_M64 : op_arr[rm_idx].type;
  }

  if (op_m(op_arr[rm_idx].type)) {
    if (rm == 4) {
      buf_write_byte(buf, EMPTY_SIB);
    }
  }

  if (rm == 5 && op_arr[rm_idx].offset == 0)
    buf_write_byte(buf, 0);

  // if (op_arr[rm_idx].offset != 0) {
  // buf_write(buf, (uint8_t *)&op_arr[rm_idx].offset, 4);
  // }
  write_offset(mod, buf, op_arr, rm_idx);
}

DEFINE_ENCODER(mr) { rm_mr_common(op_arr, buf, instr_ref, mode, ENC_MR); }
DEFINE_ENCODER(rm) { rm_mr_common(op_arr, buf, instr_ref, mode, ENC_RM); }

DEFINE_ENCODER(o) {
  const uint8_t reg = reg_lookup_val(op_arr[0].data);
  op_write_prefix(buf, op_arr, mode);

  check_mode(mode, instr_ref->support);

  uint8_t *data = &(uint8_t){*(OP_OPCODE_HELPER) + (uint8_t)reg};
  buf_write(buf, data, instr_ref->opcode_size);
}

DEFINE_ENCODER(oi) {
  o(op_arr, buf, instr_ref, mode);
  i_common(op_arr, buf, instr_ref, mode);
}

DEFINE_ENCODER(zo) {
  check_mode(mode, instr_ref->support);
  buf_write(buf, instr_ref->opcode, instr_ref->opcode_size);
}

encoder_t enc_lookup(enum enc_ident input) {
  encoder_t lookup[] = {&mr, &rm, &oi, &mi, &i, &m, &zo, &d, &o};
  return lookup[(size_t)input];
}