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

#include "instruction.h"
#include "error.h"
#include "register.h"
#include "tabs.c"
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// clang-format off

instr_encode_table_t *instr_table[] =
    {
        mov, lea, add, sub, mul, _div, and, or, xor, _not, inc,
        dec, jmp, je, jne, jz, jnz, call, ret, cmp, push, pop,
        in, out, clc, stc, cli, sti, nop, hlt, _int, syscall, 
        movzx, movsx, xchg, bswap,
    };


#define CURR_TABLE instr_table[instr.instr][j]

instr_encode_table_t instr_get_tab(instruction_t instr) {
  if (instr.instr == NULL && instr.operands == NULL) return INSTR_TAB_NULL;
  if (INSTR_DIRECTIVE(instr.instr)) return INSTR_TAB_NULL; // aka empty
  const enum operands operand_list[4] = {
      instr.operands[0].type, instr.operands[1].type,
      instr.operands[2].type, instr.operands[3].type,
  };

  // clang-format on

  enum enc_ident ident = op_ident_identify(operand_list);
  if (ident == ENC_MR && op_r(operand_list[0])) ident = ENC_RM;
  // TODO Edge case for MOV instruction - Work on in future
  if (instr.instr == INSTR_MOV)
    if (ident == ENC_MI) ident = ENC_OI;

  for (uint8_t j = 0; CURR_TABLE.opcode_size; j++)
    if (CURR_TABLE.ident == ident) return CURR_TABLE;

  // fall-through; no corresponding instruction opcode found
  err("No corrsponding instruction opcode found.");
  return INSTR_TAB_NULL; // aka empty
}

#define alloc_operand_data(type)          \
  do {                                    \
    type *type##_ = malloc(sizeof(type)); \
    *type##_ = va_arg(args, type);        \
    data = (void *)type##_;               \
  } while (0);

/* Stupid almost-stub implementation */
instruction_t instr_gen(enum instructions instr, uint8_t operand_count, ...) {
  va_list args;
  va_start(args, operand_count * 3);

  // Note, a temporary register type is used to prevent conflict
  // with the `enum registers` type by passing into `alloc_operand_data`
  typedef enum registers temp_reg;

  // clang-format off
  operand_t *operands = malloc(sizeof(operand_t) * 4);
  for (uint8_t i = 0; i < 4; i++) operands[i] = OP_NONE;
  // clang-format on

  for (uint8_t i = 0; i < operand_count; i++) {
    const enum operands type = va_arg(args, enum operands);
    char *label = "";
    void *data;
    if (op_rel(type)) {
      char *lab = va_arg(args, char *);
      label = strdup(lab);

      // clang-format off
    } else if (op_imm(type)) {
      switch (op_sizeof(type)) {
      case 8: alloc_operand_data(uint8_t); break;
      case 16: alloc_operand_data(uint16_t); break;
      case 32: alloc_operand_data(uint32_t); break;
      case 64: alloc_operand_data(uint64_t); break;
      default:
        err("Invalid operand size.");
        break;
      }
      // clang-format on
    } else {
      alloc_operand_data(temp_reg); /* Note braces as macro expands */
    }
    const size_t off = va_arg(args, size_t);
    operands[i] = op_construct_operand(type, off, data, label);
  }

  va_end(args);
  instruction_t *instr_struct = malloc(sizeof(instruction_t));
  *instr_struct = (instruction_t){.instr = instr, .operands = operands};

  return *instr_struct;
}
#undef alloc_data

instruction_t instr_write_bytes(size_t data_sz, ...) {
  buffer_t data = BUF_NULL;
  va_list args;
  va_start(args, data_sz);

  for (size_t i = 0; i < data_sz; i++) {
    const uint8_t byte = va_arg(args, uint8_t);
    buf_write_byte(&data, byte);
  }

  va_end(args);

  instruction_t *instr_ret = malloc(sizeof(instruction_t));
  // clang-format off
  *instr_ret = (instruction_t){
      .instr = INSTR_DIR_WRT_BUF,
      .operands = (operand_t[]){
          op_construct_operand(OP_MISC, 0, &data, NULL),
          OP_NONE, OP_NONE, OP_NONE,
      },
  };
  // clang-format on
  return *instr_ret;
}