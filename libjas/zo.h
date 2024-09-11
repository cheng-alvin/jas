/**
 * MIT License
 * Copyright (c) 2023-2024 John Smith (example@example.com)
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

#ifndef ZO_H
#define ZO_H

#include "buffer.h"
#include "instruction.h"
#include "mode.h"

/**
 * Dumb encoder function to encode based on the `zo` identity.
 *
 * The `zo` identity is defined as:
 * First operand | Second operand | Third operand | Fourth operand
 * --------------|----------------|---------------|--------------
 * ---           | ---            | ---           | ---
 *
 * (Basically an empty instruction for things like `syscalls` or `nop`.)
 *
 * @note Some opcodes like `syscall` can be encoded for many-indexed
 * operands, meaning there can be more than one byte for the opcode.
 */
void zo(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode);

#endif