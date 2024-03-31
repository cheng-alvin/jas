/**
 * MIT License
 * Copyright (c) 2023 Alvin Cheng (eventide1029@gmail.com)
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

#ifndef OPERAND_H
#define OPERAND_H

#include <stdint.h>

/**
 * Enumeration for the different types of operands and
 * operand sizes supported by the jas assembler.
 *
 * @note Not to be confused with `op_enc` which is used
 * only by the encoder!
 */
enum operands {
  OP_RESERVED,
  // TODO Populate the rest of the operand enumerations. 🥰
};

/**
 * Enumeration for the different types of operand encodings
 * used strictly by the encoder.
 *
 * @warning TO BE ACCESSED BY THE USER!
 *
 * @note Not to be confused with `operands` which is used
 * only by the user!
 */
enum op_enc {
  OP_REL8,
  OP_REL16,
  OP_REL32,
  // OP_PTR16_16,
  // OP_PTR16_32,
  OP_R8,
  OP_R16,
  OP_R32,
  OP_R64,
  OP_IMM8,
  OP_IMM16,
  OP_IMM32,
  OP_IMM64,
  OP_RM8,
  OP_RM16,
  OP_RM32,
  OP_RM64,
  OP_REG, // ?? Undocumented from the intel manual, re-cite information.
  OP_MEM,
  OP_MEM8,
  OP_MEM16,
  OP_MEM32,
  OP_MEM64,
  OP_SEG_REG,
  // TODO Add more operand enums if required.
};

typedef struct {
  void *data;         /* Data in the operand */
  enum operands type; /* Type tied to the void pointer*/
} operand_t;

#endif