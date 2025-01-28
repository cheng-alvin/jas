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
  ENC_NULL,
  ENC_MR,
  ENC_RM,
  ENC_OI,
  ENC_MI,
  ENC_I,
  ENC_M,
  ENC_ZO,
  ENC_D,
  ENC_O,
  ENC_IGN, /* Although **NOT** original Intel, this identity will
              void all operands, directs to the ZO identity. */
};

/**
 * Macro definition for the encoder function signature,
 * this function signature and it's parameters are all
 * documented below.
 *
 * For very very special cases where you need to define
 * a custom encoder function, or if you need to reference
 * it in a different file, you can use this macro, instead
 * of using the `enc_lookup()` function.
 *
 * @see `encoder_t`
 */
#define DEFINE_ENCODER(ident, ...) \
  void ident(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode)

/**
 * Type wrapper for the instruction encoder function pointer. Where
 * each operand encoder function takes an array of operands and
 * a buffer to write the encoded instruction to.
 *
 * (Based on the operand identities like MR, RM, etc.)
 *
 * @param op_arr The array of operands to encode
 * @param buf The buffer to write the encoded instruction to
 * @param instr_ref The instruction reference table
 * @param mode The operating mode of the instruction
 *
 * @note All encoder functions will conform to this signature.
 */
typedef void (*encoder_t)(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode);

/**
 * Lookup table for the different instruction encoder functions.
 * The lookup table is indexed by the operand encoding identity
 * and the corresponding encoder function is returned.
 *
 * @see `encoder.c`
 *
 * @param input The instruction encoding identity
 * @return The instruction encoder function pointer
 */
encoder_t enc_lookup(enum enc_ident input);

#endif