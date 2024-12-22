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
  ENC_MR,
  ENC_RM,
  ENC_OI,
  ENC_MI,
  ENC_I,
  ENC_M,
  ENC_ZO,
  ENC_D,
  ENC_O,
};

/**
 * Macro definition for the encoder function signature,
 * this function signature and it's parameters are all
 * documented in `instruction.h` with the `instr_encoder_t`
 * typedef.
 *
 * @see `instr_encoder_t`
 */
#define DEFINE_ENCODER(ident) \
  void ident(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode)

/**
 * @brief
 * The encoder function signature for the different operand
 * identifiers. The encoder functions are used to encode the
 * operands into machine code while incorporating the opcodes
 * and all other necessary information based off lookup tables
 * from `instruction.h` and the corresponding `instruction.c`.
 */

DEFINE_ENCODER(d);
DEFINE_ENCODER(i);

DEFINE_ENCODER(m);
DEFINE_ENCODER(mi);
DEFINE_ENCODER(mr);

DEFINE_ENCODER(o);
DEFINE_ENCODER(oi);

DEFINE_ENCODER(rm);
DEFINE_ENCODER(zo);

#endif