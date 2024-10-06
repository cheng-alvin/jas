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

#include "instruction.h"
#include "d.h"
#include "error.h"
#include "i.h"
#include "m.h"
#include "mi.h"
#include "mr.h"
#include "oi.h"
#include "operand.h"
#include "rm.h"
#include "zo.h"
#include <stdbool.h>
#include <stddef.h>

static void pre_default(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  if (op_sizeof(op_arr[0].type) != op_sizeof(op_arr[1].type))
    err("Invalid operand sizes.");
}

static void pre_imm(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  if (op_sizeof(op_arr[0].type) != 64) {
    pre_default(op_arr, buf, instr_ref, mode);
    return;
  }
}

instr_encode_table_t mov[] = {
    {OP_MR, NULL, {0x89}, MODE_SUPPORT_ALL, {0x88}, 1, &pre_default},
    {OP_RM, NULL, {0x8B}, MODE_SUPPORT_ALL, {0x8A}, 1, &pre_default},
    {OP_OI, NULL, {0xB8}, MODE_SUPPORT_ALL, {0xB0}, 1, &pre_default},
    {OP_MI, 0b10000000, {0xC7}, MODE_SUPPORT_ALL, {0xC6}, 1, &pre_imm},

    INSTR_TERMINATOR,
};

static void pre_lea(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  if (op_m(op_arr[0].type) || op_r(op_arr[1].type))
    err("Invalid operands type for LEA instruction.");

  if (op_sizeof(op_arr[0].type) == 8)
    err("Byte operands cannot be used with the LEA instruction.");
}

// clang-format off

#define INSTR_GENERAL(rm, rm_byte, mr, mr_byte,i, i_byte, mi_ext, mi, mi_byte)  \
      {OP_MR, NULL, {mr}, MODE_SUPPORT_ALL, {mr_byte}, 1, &pre_default},        \
      {OP_RM, NULL, {rm}, MODE_SUPPORT_ALL, {rm_byte}, 1, &pre_default},        \
      {OP_MI, mi_ext, {mi}, MODE_SUPPORT_ALL, {mi_byte}, 1, &pre_imm},          \
      {OP_I, NULL, {i}, MODE_SUPPORT_ALL, {i_byte}, 1, &pre_imm},               \
      INSTR_TERMINATOR,

// clang-format on

instr_encode_table_t lea[] = {{OP_RM, NULL, {0x8D}, MODE_SUPPORT_ALL, {0x8D}, 1, &pre_lea}, INSTR_TERMINATOR};

instr_encode_table_t add[] = {INSTR_GENERAL(0x03, 0x02, 0x01, 0x00, 0x03, 0x02, 0b10000000, 0x81, 0x80)};
instr_encode_table_t sub[] = {INSTR_GENERAL(0x2B, 0x2A, 0x28, 0x29, 0x2C, 0x2D, 5, 0x80, 0x81)};
instr_encode_table_t mul[] = {{OP_M, 4, {0xF7}, MODE_SUPPORT_ALL, {0xF6}, 1, &pre_default}, INSTR_TERMINATOR};
instr_encode_table_t div[] = {{OP_M, 6, {0xF7}, MODE_SUPPORT_ALL, {0xF6}, 1, &pre_default}, INSTR_TERMINATOR};

// Note all or, and and xor instructions have a imm8 which is not supported

instr_encode_table_t and[] = {INSTR_GENERAL(0x23, 0x22, 0x21, 0x20, 0x25, 0x24, 4, 0x81, 0x80)};
instr_encode_table_t or [] = {INSTR_GENERAL(0x0B, 0x0A, 0x09, 0x08, 0x0D, 0x0C, 1, 0x81, 0x80)};
instr_encode_table_t xor [] = {INSTR_GENERAL(0x33, 0x32, 0x31, 0x30, 0x35, 0x34, 6, 0x81, 0x80)};

// ---

instr_encode_table_t _not[] = {{OP_M, 2, {0xF7}, MODE_SUPPORT_ALL, {0xF6}, 1, &pre_default}, INSTR_TERMINATOR};

instr_encode_table_t inc[] = {{OP_M, 0, {0xFF}, MODE_SUPPORT_ALL, {0xFE}, 1, &pre_default}, INSTR_TERMINATOR};
instr_encode_table_t dec[] = {{OP_M, 1, {0xFF}, MODE_SUPPORT_ALL, {0xFE}, 1, &pre_default}, INSTR_TERMINATOR};

static void pre_jcc_no_byte(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  if (op_sizeof(op_arr[0].type) == 8)
    err("Byte operands cannot be used with this instruction.");
}

instr_encode_table_t jmp[] = {{OP_D, NULL, {0xE9}, MODE_SUPPORT_ALL, {0xEB}, 1, NULL}, INSTR_TERMINATOR};
instr_encode_table_t je[] = {{OP_D, NULL, {0x0f, 0x84}, MODE_SUPPORT_ALL, {0x090, 0x74}, 2, NULL}, INSTR_TERMINATOR};
instr_encode_table_t jne[] = {{OP_D, NULL, {0x0f, 0x85}, MODE_SUPPORT_ALL, {0x00, 0x00}, 2, &pre_jcc_no_byte}, INSTR_TERMINATOR};
instr_encode_table_t jz[] = {{OP_D, NULL, {0x0f, 0x84}, MODE_SUPPORT_ALL, {0x00, 0x00}, 2, &pre_jcc_no_byte}, INSTR_TERMINATOR};
instr_encode_table_t jnz[] = {{OP_D, NULL, {0x0f, 0x85}, MODE_SUPPORT_ALL, {0x90, 0x75}, 2, NULL}, INSTR_TERMINATOR};

instr_encode_table_t call[] = {{}};

static void pre_ret(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  if (op_sizeof(op_arr[0].type) != 16)
    err("Other operand sizes cannot be used with this instruction.");
}

// TODO / note far jumps, calls and returns are not supported (yet)
instr_encode_table_t ret[] = {{
    {OP_ZO, NULL, {0xC3}, MODE_SUPPORT_ALL, {0xC3}, 1, &pre_default},
    {OP_I, NULL, {0xC2}, MODE_SUPPORT_ALL, {0xC2}, 1, &pre_ret},
}};

instr_encode_table_t cmp[] = {{}};

instr_encode_table_t push[] = {{}};
instr_encode_table_t pop[] = {{}};

instr_encode_table_t in[] = {{}};
instr_encode_table_t out[] = {{}};

instr_encode_table_t clc[] = {{}};
instr_encode_table_t stc[] = {{}};
instr_encode_table_t cli[] = {{}};
instr_encode_table_t sti[] = {{}};

instr_encode_table_t nop[] = {{OP_ZO, NULL, {0x90}, MODE_SUPPORT_ALL, {0x90}, 1, &pre_default}, INSTR_TERMINATOR};
instr_encode_table_t hlt[] = {{OP_ZO, NULL, {0xF4}, MODE_SUPPORT_ALL, {0xF4}, 1, &pre_default}, INSTR_TERMINATOR};

static void pre_int(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  if (op_sizeof(op_arr[0].type) != 8)
    err("Invalid operand size for INT instruction.");
}

instr_encode_table_t _int[] = {{OP_I, NULL, {0xCD}, MODE_SUPPORT_ALL, {0xCD}, 1, &pre_int}, INSTR_TERMINATOR};
instr_encode_table_t syscall[] = {{OP_ZO, NULL, {0x0F, 0x05}, MODE_SUPPORT_64BIT, {0x00, 0x00}, 2, &pre_default}, INSTR_TERMINATOR};

// clang-format off

instr_encode_table_t *instr_table[] =
    {
        mov, lea, add, sub, mul, div, and, or, xor, _not, inc,
        dec, jmp, je, jne, jz, jnz, call, ret, cmp, push, pop,
        in, out, clc, stc, cli, sti, nop, hlt, _int, syscall,
    };

// clang-format on

instr_encoder_t instr_encode_func(enum op_ident input) {
  instr_encoder_t lookup[] = {&mr, &rm, &oi, &mi, &i, &m, &zo, &d};
  return lookup[(size_t)input];
}
