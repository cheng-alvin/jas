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

#ifndef I_H
#define I_H

#include "instruction.h"
#include "operand.h"

/**
 * Dumb encoder function to encode based on the `i` identity.
 *
 * The `i` identity is defined as:
 * First operand | Second operand | Third operand | Fourth operand
 * --------------|----------------|---------------|--------------
 * AL/AX/EAX/RAX | IMM            | ---           | ---
 *
 * Reference key:
 * - IMM denotes an immediate integer value ranging from byte to
 * quad-word sizes.
 *
 * @note The first operand is a register that can be either AL, AX, EAX
 * or their 64 bit counterpart, RAX. This encoder improves on the code
 * size by inferring addition usage the accumulator. :)
 *
 * @note For notes on the arguments see `instr_encoder_t`.
 * @extends instr_encoder_t
 */
void i(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode);

#endif