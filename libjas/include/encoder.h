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

#ifndef ENCODER_H
#define ENCODER_H

#include "buffer.h"
#include "mode.h"

// Forward declarations -  See `instruction.h` and `operand.h` respectively
typedef struct instr_encode_table instr_encode_table_t;
typedef struct operand operand_t;

/**
 * Enumeration for the different operand identifiers.
 * Used to lookup the operand encoder functions.
 *
 * @note See identities and their corresponding encoder
 * methods in the main source file - `encoder.c`.
 */
enum enc_ident {
  OP_MR,
  OP_RM,
  OP_OI,
  OP_MI,
  OP_I,
  OP_M,
  OP_ZO,
  OP_D,
  OP_O,
};

/**
 * @brief
 * The encoder function signature for the different operand
 * identifiers. The encoder functions are used to encode the
 * operands into machine code while incorporating the opcodes
 * and all other necessary information based off lookup tables
 * from `instruction.h` and the corresponding `instruction.c`.
 */

void d(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode);
void i(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode);

void m(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode);
void mi(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode);
void mr(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode);

void o(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode);
void oi(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode);

void rm(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode);

void zo(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode);

#endif