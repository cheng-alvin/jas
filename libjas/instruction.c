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
#include "error.h"
#include "i.h"
#include "m.h"
#include "mi.h"
#include "mr.h"
#include "oi.h"
#include "operand.h"
#include "rm.h"
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
    {OP_OI, NULL, {0xB8}, MODE_SUPPORT_ALL, {0xB0}, 1, &pre_imm},
    {OP_MI, 0b10000000, {0xC7}, MODE_SUPPORT_ALL, {0xC6}, 1, &pre_imm},

    INSTR_TERMINATOR,
};

static void pre_lea(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  if (op_m(op_arr[0].type) || op_r(op_arr[1].type))
    err("Invalid operands type for LEA instruction.");

  if (op_sizeof(op_arr[0].type) == 8)
    err("Byte operands cannot be used with the LEA instruction.");
}

instr_encode_table_t lea[] = {{OP_RM, NULL, {0x8D}, MODE_SUPPORT_ALL, {0x8D}, 1, &pre_lea}, INSTR_TERMINATOR};

instr_encode_table_t add[] = {
    {OP_RM, NULL, {0x03}, MODE_SUPPORT_ALL, {0x02}, 1, &pre_default},
    {OP_MR, NULL, {0x01}, MODE_SUPPORT_ALL, {0x00}, 1, &pre_default},
    {OP_MI, 0b10000000, {0x81}, MODE_SUPPORT_ALL, {0x80}, 1, &pre_imm},
    {OP_I, NULL, {0x03}, MODE_SUPPORT_ALL, {0x02}, 1, &pre_imm},

    INSTR_TERMINATOR,

};

instr_encode_table_t sub[] = {
    {OP_RM, NULL, {0x2B}, MODE_SUPPORT_ALL, {0x2A}, 1, &pre_default},
    {OP_MR, NULL, {0x28}, MODE_SUPPORT_ALL, {0x29}, 1, &pre_default},
    {OP_MI, 5, {0x80}, MODE_SUPPORT_ALL, {0x81}, 1, &pre_imm},
    {OP_I, NULL, {0x2C}, MODE_SUPPORT_ALL, {0x2D}, 1, &pre_imm},

    INSTR_TERMINATOR,
};

instr_encode_table_t mul[] = {{OP_M, 4, {0xF7}, MODE_SUPPORT_ALL, {0xF6}, 1, &pre_default}, INSTR_TERMINATOR};
instr_encode_table_t div[] = {{OP_M, 6, {0xF7}, MODE_SUPPORT_ALL, {0xF6}, 1, &pre_default}, INSTR_TERMINATOR};

// Note all or, and and xor instructions have a imm8 which is not supported

instr_encode_table_t and[] = {
    {OP_RM, NULL, {0x23}, MODE_SUPPORT_ALL, {0x22}, 1, &pre_default},
    {OP_MR, NULL, {0x21}, MODE_SUPPORT_ALL, {0x20}, 1, &pre_default},
    {OP_MI, 4, {0x81}, MODE_SUPPORT_ALL, {0x80}, 1, &pre_imm},
    {OP_I, NULL, {0x25}, MODE_SUPPORT_ALL, {0x24}, 1, &pre_imm},

    INSTR_TERMINATOR,
};

instr_encode_table_t or [] = {
    {OP_RM, NULL, {0x0B}, MODE_SUPPORT_ALL, {0x0A}, 1, &pre_default},
    {OP_MR, NULL, {0x09}, MODE_SUPPORT_ALL, {0x08}, 1, &pre_default},
    {OP_MI, 1, {0x81}, MODE_SUPPORT_ALL, {0x80}, 1, &pre_imm},
    {OP_I, NULL, {0x0D}, MODE_SUPPORT_ALL, {0x0C}, 1, &pre_imm},

    INSTR_TERMINATOR,
};

instr_encode_table_t xor [] = {
    {OP_RM, NULL, {0x33}, MODE_SUPPORT_ALL, {0x32}, 1, &pre_default},
    {OP_MR, NULL, {0x31}, MODE_SUPPORT_ALL, {0x30}, 1, &pre_default},
    {OP_MI, 6, {0x81}, MODE_SUPPORT_ALL, {0x80}, 1, &pre_imm},
    {OP_I, NULL, {0x35}, MODE_SUPPORT_ALL, {0x34}, 1, &pre_imm},

    INSTR_TERMINATOR,
};

// ---

instr_encode_table_t _not[] = {{OP_M, 2, {0xF7}, MODE_SUPPORT_ALL, {0xF6}, 1, &pre_default}, INSTR_TERMINATOR};

instr_encode_table_t inc[] = {{OP_M, 0, {0xFF}, MODE_SUPPORT_ALL, {0xFE}, 1, &pre_default}, INSTR_TERMINATOR};
instr_encode_table_t dec[] = {{OP_M, 1, {0xFF}, MODE_SUPPORT_ALL, {0xFE}, 1, &pre_default}, INSTR_TERMINATOR};

instr_encode_table_t *instr_table[] =
    {mov, lea, add, sub, mul, div, and, or, xor, _not, inc, dec};

instr_encoder_t instr_encode_func(enum op_ident input) {
  instr_encoder_t lookup[] = {&mr, &rm, &oi, &mi, &i, &m};
  return lookup[(size_t)input];
}
