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

#ifndef OPERAND_HPP
#define OPERAND_HPP

#include <cstdint>

using namespace std;

/**
 * Type wrapper for an unsigned char that represents the
 * hash of an operand identifier. Used as a value for comparison
 * against the operand identity encoder lookup table. The individual
 * bits of the byte represent the type of operands within the
 * operand identifier as follows:
 *
 * 0: Relative operand (rel8/16/32)
 * 1: Register operand (r8/16/32/64)
 * 2: Immediate operand (imm8/16/32/64)
 * 3: Memory operand (m8/16/32/64)
 * 4: -- Reserved for future use --
 * 5: Accumulator register operand (acc8/16/32/64)
 *
 * 6-7: Reserved for future use
 *
 * A bit set to 1 indicates that the operand is of the corresponding
 * type, while a bit set to 0 indicates that the operand is not of the
 * corresponding type.
 *
 * @note Not to be confused with the operand types (`enum operands`)
 *
 * Also, there are macros below to help you!
 */
typedef uint8_t op_ident_hash_t;

/**
 * Macro definitions for the different operand hash values.
 * Used to compare against the operand identity encoder lookup table.
 *
 * @see `op_ident_hash_t`
 */

#define OP_HASH_REL 0b00000001
#define OP_HASH_R 0b00000010
#define OP_HASH_IMM 0b00000100
#define OP_HASH_M 0b00001000
#define OP_HASH_ACC 0b00100000

#endif
