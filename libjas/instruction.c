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
#include <stdarg.h>
#include <stddef.h>

static void same_operand_sizes(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  const uint8_t ref = op_sizeof(op_arr[0].type);

  for (uint8_t i = 0; i < 4; i++) {
    if (op_arr[i].type == OP_NULL) continue;

    if (op_sizeof(op_arr[i].type) != ref) {
      err("Invalid operand sizes.");
      break;
    }
  }
}

static void pre_imm(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  if (op_sizeof(op_arr[0].type) != 64) {
    same_operand_sizes(op_arr, buf, instr_ref, mode);
    return;
  }
}

#define ZERO_EXT 0b10000000
#define DEFINE_TAB(name) instr_encode_table_t name[]

DEFINE_TAB(mov) = {
    {ENC_MR, NULL, {0x89}, MODE_SUPPORT_ALL, {0x88}, 1, &same_operand_sizes, true},
    {ENC_RM, NULL, {0x8B}, MODE_SUPPORT_ALL, {0x8A}, 1, &same_operand_sizes, true},
    {ENC_OI, NULL, {0xB8}, MODE_SUPPORT_ALL, {0xB0}, 1, &same_operand_sizes},
    {ENC_MI, ZERO_EXT, {0xC7}, MODE_SUPPORT_ALL, {0xC6}, 1, &pre_imm, true},

    INSTR_TERMINATOR,
};

static void pre_lea(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  if (op_m(op_arr[0].type) || op_r(op_arr[1].type))
    err("Invalid operands type for LEA instruction.");

  if (op_sizeof(op_arr[0].type) == 8)
    err("Byte operands cannot be used with the LEA instruction.");
}

static void no_operands(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  for (uint8_t i = 0; i < 4; i++) {
    if (op_arr[i].type != OP_NULL)
      err("This encoder identity does not support any operands.");
  }
}

// clang-format off

#define GENERIC(rm, rm_byte, mr, mr_byte,i, i_byte, mi_ext, mi, mi_byte)  \
     {ENC_RM, NULL, {rm}, MODE_SUPPORT_ALL, {rm_byte}, 1, &same_operand_sizes, true},        \
      {ENC_MR, NULL, {mr}, MODE_SUPPORT_ALL, {mr_byte}, 1, &same_operand_sizes, true},        \
      {ENC_I, NULL, {i}, MODE_SUPPORT_ALL, {i_byte}, 1, &pre_imm, true},               \
      {ENC_MI, mi_ext, {mi}, MODE_SUPPORT_ALL, {mi_byte}, 1, &pre_imm, true},          \
      INSTR_TERMINATOR,

// clang-format on

DEFINE_TAB(lea) = {{ENC_RM, NULL, {0x8D}, MODE_SUPPORT_ALL, {0x8D}, 1, &pre_lea, true}, INSTR_TERMINATOR};

DEFINE_TAB(add) = {GENERIC(0x03, 0x02, 0x01, 0x00, 0x03, 0x02, ZERO_EXT, 0x81, 0x80)};
DEFINE_TAB(sub) = {GENERIC(0x2B, 0x2A, 0x28, 0x29, 0x2C, 0x2D, 5, 0x81, 0x80)};
DEFINE_TAB(mul) = {{ENC_M, 4, {0xF7}, MODE_SUPPORT_ALL, {0xF6}, 1, &same_operand_sizes, true}, INSTR_TERMINATOR};
DEFINE_TAB(div) = {{ENC_M, 6, {0xF7}, MODE_SUPPORT_ALL, {0xF6}, 1, &same_operand_sizes, true}, INSTR_TERMINATOR};

// Note all or, and and xor instructions have a imm8 which is not supported

DEFINE_TAB(and) = {GENERIC(0x23, 0x22, 0x21, 0x20, 0x25, 0x24, 4, 0x81, 0x80)};
DEFINE_TAB(or) = {GENERIC(0x0B, 0x0A, 0x09, 0x08, 0x0D, 0x0C, 1, 0x81, 0x80)};
DEFINE_TAB(xor) = {GENERIC(0x33, 0x32, 0x31, 0x30, 0x35, 0x34, 6, 0x81, 0x80)};

// ---

DEFINE_TAB(_not) = {{ENC_M, 2, {0xF7}, MODE_SUPPORT_ALL, {0xF6}, 1, &same_operand_sizes, true}, INSTR_TERMINATOR};

DEFINE_TAB(inc) = {{ENC_M, 0, {0xFF}, MODE_SUPPORT_ALL, {0xFE}, 1, &same_operand_sizes, true}, INSTR_TERMINATOR};
DEFINE_TAB(dec) = {{ENC_M, 1, {0xFF}, MODE_SUPPORT_ALL, {0xFE}, 1, &same_operand_sizes, true}, INSTR_TERMINATOR};

static void pre_jcc_no_byte(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  if (op_sizeof(op_arr[0].type) == 8)
    err("Byte operands cannot be used with this instruction.");
}

DEFINE_TAB(jmp) = {
    {ENC_D, NULL, {0xE9}, MODE_SUPPORT_ALL, {0xEB}, 1, NULL, true},
    {ENC_M, 4, {0xFF}, MODE_SUPPORT_ALL, {NULL}, 1, &pre_imm, false},
    INSTR_TERMINATOR,
};

DEFINE_TAB(je) = {{ENC_D, NULL, {0x0f, 0x84}, MODE_SUPPORT_ALL, {0x090, 0x74}, 2, NULL, true}, INSTR_TERMINATOR};
DEFINE_TAB(jne) = {{ENC_D, NULL, {0x0f, 0x85}, MODE_SUPPORT_ALL, {0x00, 0x00}, 2, &pre_jcc_no_byte, true}, INSTR_TERMINATOR};
DEFINE_TAB(jz) = {{ENC_D, NULL, {0x0f, 0x84}, MODE_SUPPORT_ALL, {0x00, 0x00}, 2, &pre_jcc_no_byte, true}, INSTR_TERMINATOR};
DEFINE_TAB(jnz) = {{ENC_D, NULL, {0x0f, 0x85}, MODE_SUPPORT_ALL, {0x90, 0x75}, 2, NULL, true}, INSTR_TERMINATOR};

DEFINE_TAB(call) = {
    {ENC_D, NULL, {0xE8}, MODE_SUPPORT_ALL, {0xEB}, 1, NULL, true},
    {ENC_M, 2, {0xFF}, MODE_SUPPORT_ALL, {NULL}, 1, &pre_imm, false},
    INSTR_TERMINATOR,
};

static void pre_ret(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  if (op_sizeof(op_arr[0].type) != 16)
    err("Other operand sizes cannot be used with this instruction.");
}

// TODO / note far jumps, calls and returns are not supported (yet)
DEFINE_TAB(ret) = {
    {ENC_ZO, NULL, {0xC3}, MODE_SUPPORT_ALL, {0xC3}, 1, &no_operands, true},
    {ENC_I, NULL, {0xC2}, MODE_SUPPORT_ALL, {0xC2}, 1, &pre_ret, true},
    INSTR_TERMINATOR,
};

DEFINE_TAB(cmp) = {GENERIC(0x3B, 0x3A, 0x39, 0x38, 0x3D, 0x3C, 7, 0x81, 0x80)};

DEFINE_TAB(push) = {
    {ENC_M, 6, {0xFF}, MODE_SUPPORT_ALL, NULL, 1, NULL, false},
    {ENC_O, NULL, {0x50}, MODE_SUPPORT_ALL, NULL, 1, NULL, false},
    {ENC_I, NULL, {0x68}, MODE_SUPPORT_ALL, {0x6A}, 1, &pre_imm, true},
    INSTR_TERMINATOR,
};

DEFINE_TAB(pop) = {
    {ENC_M, 0, {0x8F}, MODE_SUPPORT_ALL, NULL, 1, NULL, false},
    {ENC_O, NULL, {0x58}, MODE_SUPPORT_ALL, NULL, 1, NULL, false},
    INSTR_TERMINATOR,
};

DEFINE_TAB(in) = {{}};
DEFINE_TAB(out) = {{}};

DEFINE_TAB(clc) = {{ENC_ZO, NULL, {0xF8}, MODE_SUPPORT_ALL, {NULL}, 1, &no_operands, false}, INSTR_TERMINATOR};
DEFINE_TAB(stc) = {{ENC_ZO, NULL, {0xF9}, MODE_SUPPORT_ALL, {NULL}, 1, &no_operands, false}, INSTR_TERMINATOR};
DEFINE_TAB(cli) = {{ENC_ZO, NULL, {0xFA}, MODE_SUPPORT_ALL, {NULL}, 1, &no_operands, false}, INSTR_TERMINATOR};
DEFINE_TAB(sti) = {{ENC_ZO, NULL, {0xFB}, MODE_SUPPORT_ALL, {NULL}, 1, &no_operands, false}, INSTR_TERMINATOR};

DEFINE_TAB(nop) = {{ENC_ZO, NULL, {0x90}, MODE_SUPPORT_ALL, {NULL}, 1, &no_operands, false}, INSTR_TERMINATOR};
DEFINE_TAB(hlt) = {{ENC_ZO, NULL, {0xF4}, MODE_SUPPORT_ALL, {0xF4}, 1, &no_operands, true}, INSTR_TERMINATOR};

static void pre_int(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  if (op_sizeof(op_arr[0].type) != 8)
    err("Invalid operand size for INT instruction.");
}

DEFINE_TAB(_int) = {{ENC_I, NULL, {0xCD}, MODE_SUPPORT_ALL, {NULL}, 1, &pre_int, false}, INSTR_TERMINATOR};
DEFINE_TAB(syscall) = {
    {ENC_ZO, NULL, {0x0F, 0x05}, MODE_SUPPORT_64BIT, {NULL}, 2, &same_operand_sizes, false},
    INSTR_TERMINATOR,
};

static void pre_small_operands(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  if (op_sizeof(op_arr[1].type) < 16)
    err("Invalid operand size for MOVZX/MOVSX instruction");
}

DEFINE_TAB(movzx) = {{ENC_RM, NULL, {0x0F, 0xB7}, MODE_SUPPORT_ALL, {0x0F, 0xB6}, 2, &pre_small_operands, true}, INSTR_TERMINATOR};
DEFINE_TAB(movsx) = {{ENC_RM, NULL, {0x0F, 0xBF}, MODE_SUPPORT_ALL, {0x0F, 0xBE}, 2, &pre_small_operands, true}, INSTR_TERMINATOR};

DEFINE_TAB(xchg) = {
    {ENC_O, NULL, {0x90}, MODE_SUPPORT_ALL, {NULL}, 1, &same_operand_sizes, false},
    {ENC_MR, NULL, {0x87}, MODE_SUPPORT_ALL, {0x86}, 1, &same_operand_sizes, true},
    {ENC_RM, NULL, {0x87}, MODE_SUPPORT_ALL, {0x86}, 1, &same_operand_sizes, true},
    INSTR_TERMINATOR,
};

// clang-format off

instr_encode_table_t *instr_table[] =
    {
        mov, lea, add, sub, mul, div, and, or, xor, _not, inc,
        dec, jmp, je, jne, jz, jnz, call, ret, cmp, push, pop,
        in, out, clc, stc, cli, sti, nop, hlt, _int, syscall, 
        movzx, movsx, xchg,
    };


#define CURR_TABLE instr_table[instr.instr][j]

instr_encode_table_t instr_get_tab(instruction_t instr) {
  if (instr.instr == NULL && instr.operands == NULL) return INSTR_TERMINATOR;
  if (IS_LABEL(instr)) return INSTR_TERMINATOR; // aka empty
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
  return INSTR_TERMINATOR; // aka empty
}

instruction_t instr_gen(enum instructions instr, uint8_t operand_count, ...) {
  va_list args;
  va_start(args, operand_count * 3);

  operand_t operands[4] = {OP_NONE, OP_NONE, OP_NONE, OP_NONE};
  for (uint8_t i = 0; i < operand_count; i++) {
    const enum operands type = va_arg(args, enum operands);
    char *label = "";
    void *data;
    if (op_rel(type)) {
      char *lab = va_arg(args, char *);
      label = lab;
    } else if (op_imm(type)) {
      // clang-format off
      switch (op_sizeof(type)) {
      case 8: data = &(uint8_t){va_arg(args, uint8_t)}; break;
      case 16: data = &(uint16_t){va_arg(args, uint16_t)}; break;
      case 32: data = &(uint32_t){va_arg(args, uint32_t)}; break;
      case 64: data = &(uint64_t){va_arg(args, uint64_t)}; break;
      default:
        err("Invalid operand size.");
        break;
      }
      // clang-format on
    } else
      data = (void *)&(enum registers){va_arg(args, enum registers)};

    const size_t off = va_arg(args, size_t);
    operands[i] = op_construct_operand(type, off, data, label);
  }

  va_end(args);
  return (instruction_t){
      .instr = instr,
      .operands = operands,
  };
}