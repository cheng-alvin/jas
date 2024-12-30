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

#include "operand.h"
#include "error.h"
#include "instruction.h"
#include "register.h"
#include "rex.h"
#include <stdbool.h>

uint8_t op_modrm_mode(operand_t input) {
  register const enum registers deref_reg = (*(enum registers *)input.data);
  if (reg_lookup_val(input.data) == 5 && !reg_needs_rex(deref_reg)) {
    if (deref_reg == REG_RIP || deref_reg == REG_EIP || deref_reg == REG_IP) {
      if (op_r(deref_reg))
        err("RIP, EIP and IP cannot be used as direct operands.");
      return OP_MODRM_INDIRECT;
    } else if (input.offset == 0)
      return OP_MODRM_DISP8; // Assume to be Base pointer reg
  }

  if (op_m(input.type) && input.offset == 0)
    return OP_MODRM_INDIRECT;

  if (input.offset != 0) {
    if ((intmax_t)input.offset > INT32_MAX) err("Displacement value is too large.");
    if ((intmax_t)input.offset > INT8_MAX) // Size of a `uint8_t`
      return OP_MODRM_DISP32;

    return OP_MODRM_DISP8; // Revert to 8-bit displacement when extra space is not needed
  }

  return OP_MODRM_REG;
}

uint8_t op_sizeof(enum operands input) {
  if (op_byte(input)) return 8;
  if (op_word(input)) return 16;
  if (op_dword(input)) return 32;
  if (op_qword(input)) return 64;

  return 0;
}

uint8_t *op_write_opcode(operand_t *op_arr, instr_encode_table_t *instr_ref) {
  if (!instr_ref->has_byte_opcode) return instr_ref->opcode;

  // According to Oracle's <x86 Assembly Language Reference Manual>
  // (Yeah seriously - Oracle has a x86 Assembly Language Reference Manual)
  // Only when *BOTH* operands are byte-sized, the opcode will also be byte-sized
  // So, wel'll check if they are *BOTH* byte sized then write that.

  const uint8_t reference = op_sizeof(op_arr[0].type);
  const uint8_t sizes[] =
      {reference, op_sizeof(op_arr[1].type),
       op_sizeof(op_arr[2].type), op_sizeof(op_arr[3].type)};

  bool sizes_are_byte = reference == 8;
  for (unsigned char i = 0; i < 4; i++) {
    if (op_arr[i].type == OP_NULL) break;
    if (sizes[i] != 8 && sizes_are_byte) sizes_are_byte = false;
  }

  if (sizes_are_byte) return instr_ref->byte_instr_opcode;
  return instr_ref->opcode;
}

void op_write_prefix(buffer_t *buf, const operand_t *op_arr, enum modes mode) {
  /**
   * @brief
   * The 0x66 and 0x67 hex prefixes are the operand and address size
   * overrides respectively. They are used to change the size of the operand
   * or address in the instruction. Basically, if there's no prefix, the op-
   * erand as well as the address size is the same as the mode's bit size.
   * So long mode is 64 and so on.
   *
   * However, the override prefixes can change the size of the operand or
   * address to 16 bits in protected mode, or 32 bits in real mode. This is
   * useful for instructions that require a specific operand size.
   *
   * mode | size    | output
   * -----------------------------
   * real | 32      | 0x66
   * real | 32 addr | 0x67
   * prot | 16      | 0x66
   * prot | 16 addr | 0x67
   * long | 16      | 0x66
   * long | 32 addr | 0x67
   * long | 16 addr | 0x67 + 0x66
   *
   * long | 64      | REX.W
   *
   * Additional: Register REX prefix for REX.B
   *
   * @note 64-bit operands are not allowed in any modes other than long mode
   * hence, if a 64-bit operand is present throw an error if other than
   * long mode
   *
   */

  uint8_t already_written = 0;
  uint8_t rex = REX_DEFAULT;

  for (uint8_t i = 0; i < 4; i++) {
    if (op_arr[i].type == OP_NULL)
      continue;

    const uint8_t size = op_sizeof(op_arr[i].type);
    uint8_t override = op_m(op_arr[i].type) ? OP_ADDR_OVERRIDE : OP_WORD_OVERRIDE;

    if (op_rm(op_arr[i].type))
      rex |= reg_needs_rex(op_arr[i].data) ? REX_B : 0;

    switch (size) {
    case 16:
      if (mode == MODE_REAL) break;
      if (mode == MODE_LONG && op_m(op_arr[i].type)) {
        if (!buf_element_exists(buf, OP_WORD_OVERRIDE))
          buf_write_byte(buf, OP_WORD_OVERRIDE);
      }

      goto override_write;

    case 32:
      if (mode == MODE_REAL) goto override_write;
      if (override != OP_ADDR_OVERRIDE || mode == MODE_PROTECTED) break;

    override_write:
      if (already_written == override) break;
      buf_write_byte(buf, override);
      already_written = override;
      break;

    case 64:
      if (mode != MODE_LONG) {
        err("64-bit operands are prohibited in modes other than long mode.");
        break;
      }

      rex |= REX_W;
      break;

    default:
      break;
    }
  }

  if (rex != REX_DEFAULT)
    buf_write_byte(buf, rex);
}

operand_t op_construct_operand(enum operands type, size_t offset, void *data) {
  return (operand_t){
      .type = type,
      .offset = offset,
      .data = data,
  };
}
