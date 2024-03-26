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

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "operand.h"
#include <stdint.h>

/**
 * Enumeration for the different types of instructions
 * supported by the jas assembler.
 */
enum instructions {
  INSTR_MOV,
  INSTR_LEA,
  INSTR_ADD,
  INSTR_SUB,
  INSTR_MUL,
  INSTR_DIV,
  INSTR_AND,
  INSTR_OR,
  INSTR_XOR,
  INSTR_NOT,
  INSTR_INC,
  INSTR_DEC,
  INSTR_JMP,
  INSTR_JE,
  INSTR_JNE,
  INSTR_JZ,
  INSTR_JNZ,
  INSTR_CALL,
  INSTR_RET,
  INSTR_CMP,
  INSTR_PUSH,
  INSTR_POP,
  INSTR_MOVS,
  INSTR_LODS,
  INSTR_STOS,
  INSTR_CMPS,
  INSTR_SCAS,
  INSTR_IN,
  INSTR_OUT,
  INSTR_CLC,
  INSTR_STC,
  INSTR_CLI,
  INSTR_STI,
  INSTR_NOP,
  INSTR_HLT,
  INSTR_INT,
  INSTR_SYSCALL,
};

/**
 * Type wrapper for an unsigned char that represents the
 * support status of an instruction in different intel x86
 * operation modes, like 64, 32 and 16 bit modes. The individual
 * bits of the byte represent the support status of the instruction
 * in the different operation modes. The bits are ordered from
 * least significant to most significant as follows:
 *
 * 0: 16 bit mode encoding support status
 * 1: 32 bit mode encoding support status
 * 2: 64 bit mode encoding support status
 *
 * 3-7: Reserved for future use
 *
 * A bit set to 1 indicates that the instruction is supported
 * in the corresponding operation mode, while a bit set to 0
 * indicates that the instruction is not supported in the
 * corresponding operation mode.
 *
 * 🤠
 */
typedef uint8_t instr_support_t;

typedef struct {
  enum instructions type;      /* Type of the instruction */
  enum operands *operands;     /* List of operands */
  uint8_t *opcode;             /* Opcode of the instruction */
  uint8_t *prefixes;           /* Prefixes of the instruction */
  instr_support_t support_stat /* Operation mode encoding support status */
} instruction_t;

#endif