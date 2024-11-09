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

#ifndef OPERAND_H
#define OPERAND_H

#include "buffer.h"
#include "encoder.h"
#include "mode.h"
#include <stdint.h>

/**
 * Definitions to the macros used in the operand encoder functions.
 * to represent the addressing modes of the ModR/M byte.
 *
 * Basically, the ModR/M byte, in a nutshell is a byte that gives
 * the addressing mode of the operand, register and/or memory
 * data associated with the current operation.
 *
 * They definitely did a way better job explaining it here:
 * @see https://wiki.osdev.org/X86-64_Instruction_Encoding#ModR.2FM_Byte
 */

#define OP_MODRM_INDIRECT 0b00000000
#define OP_MODRM_DISP8 0b01000000
#define OP_MODRM_DISP32 0b10000000
#define OP_MODRM_REG 0b11000000

/**
 * Macro definition for the 16-bit operand override byte for supporting
 * word-sized operands and addresses in the x86 family.
 *
 * @see https://stackoverflow.com/questions/74954166/
 */
#define OP_WORD_OVERRIDE 0x66
#define OP_ADDR_OVERRIDE 0x67

/**
 * Enumeration for the different types of operands and
 * operand sizes supported by the jas assembler.
 */
enum operands {
  OP_NULL,
  OP_MISC,
  OP_REL8,
  OP_REL16,
  OP_REL32,
  OP_R8,
  OP_R16,
  OP_R32,
  OP_R64,
  OP_IMM8,
  OP_IMM16,
  OP_IMM32,
  OP_IMM64,
  OP_M8,
  OP_M16,
  OP_M32,
  OP_M64,
  OP_ACC8,
  OP_ACC16,
  OP_ACC32,
  OP_ACC64,
};

/**
 * Definitions for the different macros responsible of checking
 * if the operand is a relative, register, immediate, memory,
 * depending on it's specific enumeration.
 *
 * (In other words, it just "generalizes" the specific operand enums)
 *
 * Btw, there's also general macros for finding the size of the
 * operand in assembly!
 *
 * @note You can just slop it between an if statement like so:
 * @example if (op_rel(x)) { ... }
 */

#define op_rel(x) ((x) <= OP_REL32 && (x) >= OP_REL8)
#define op_r(x) ((x) <= OP_R64 && (x) >= OP_R8)
#define op_imm(x) ((x) <= OP_IMM64 && (x) >= OP_IMM8)
#define op_m(x) ((x) <= OP_M64 && (x) >= OP_M8)
#define op_acc(x) ((x) <= OP_ACC64 && (x) >= OP_ACC8)

// --

#define op_byte(x) (x == OP_REL8 || x == OP_R8 || x == OP_IMM8 || x == OP_M8 || x == OP_ACC8)
#define op_word(x) (x == OP_REL16 || x == OP_R16 || x == OP_IMM16 || x == OP_M16 || x == OP_ACC16)
#define op_dword(x) (x == OP_REL32 || x == OP_R32 || x == OP_IMM32 || x == OP_M32 || x == OP_ACC32)
#define op_qword(x) (x == OP_R64 || x == OP_IMM64 || x == OP_M64 || x == OP_ACC64)

// --

#define op_rm(x) (op_r(x) || op_m(x))

typedef struct {
  void *data;         /* Data in the operand */
  enum operands type; /* Type tied to the void pointer*/
  uint32_t offset;    /* The offset applied to the data (if applicable) */
  char *label;        /* The name of a referenced label (if applicable) */
} operand_t;

/**
 * Function for setting the prefix of the operand based on the
 * size of the reference operand array, and writes it the
 * buffer provided in `buf`.
 *
 * @param buf The pointer to the buffer to write the prefixes to.
 * @param op_arr The operand array to base the prefixes from.
 * @param mode The current operating mode of the instruction
 */
void op_write_prefix(buffer_t *buf, const operand_t *op_arr, enum modes mode);

/**
 * Function for identifying the operand identity, created using
 * a large c++ unordered_map.
 *
 * @param input The input operand list
 * @return The operand identity enumeration
 */
enum enc_ident op_ident_identify(enum operands *input);

/**
 * Simple function for determining the ModR/M mode based on the
 * operand type and offset provided by the operand struct.
 * (As defined below)
 *
 * @param input The operand struct to determine the mode from
 * @return The ModR/M mode
 *
 * @see `operand_t`
 */
uint8_t op_modrm_mode(operand_t input);

/**
 * Function for finding the size of the operand type in bits, very
 * similar to the `sizeof` operator in C99. Used to determine
 * the size of the operand in the instruction.
 *
 * @see https://en.cppreference.com/w/c/types/sizeof
 * @see `enum operands`
 *
 * @param input The operand identifier in enum form
 * @return The size of the operand in bits
 *
 * @note returns 0 if the operand size is not recognized
 *
 * @note This function checks for the size of the OPERAND TYPE
 * NOT THE OPERAND DATA ITSELF!
 */
uint8_t op_sizeof(enum operands input);

/**
 * Function for constructing an operand struct with the given
 * type, offset and data. Therefore, prevents the temptation
 * to make things difficult by using the struct initializer
 * as well as mangling around with void pointers.
 *
 * @param type The type of the operand
 * @param offset The offset of the operand
 * @param data The data of the operand
 * @return The constructed operand struct
 *
 * @note Operands and parameter types are based on `operand_t`
 * @see `operand_t`
 */
operand_t op_construct_operand(enum operands type, size_t offset, void *data);

#define OP_NONE      \
  (operand_t) {      \
    .data = NULL,    \
    .type = OP_NULL, \
    .offset = 0,     \
  }

#endif
