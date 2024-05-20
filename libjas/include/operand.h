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

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OPERAND_H
#define OPERAND_H

#include "buffer.h"
#include "instruction.h"

/**
 * Definitions to the macros used in the operand encoder functions.
 * to represent the addressing modes of the ModR/M byte.
 *
 * It's a very hard encoding thing to understand, therefore:
 * @see https://wiki.osdev.org/X86-64_Instruction_Encoding#ModR.2FM_Byte
 */

#define OP_MODRM_INDIRECT 0b00000000
#define OP_MODRM_DISP8 0b01000000
#define OP_MODRM_DISP32 0b10000000
#define OP_MODRM_REG 0b11000000

/**
 * Macro definition for the 16-bit operand override byte for supporting
 * word-sized operands in the x86 family.
 *
 * @see https://stackoverflow.com/questions/74954166/
 */
#define OP_WORD_OVERRIDE 0x66

/**
 * Enumeration for the different types of operands and
 * operand sizes supported by the jas assembler.
 */
enum operands {
  OP_NULL,
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
  OP_SEG_REG,
  OP_ACC8,
  OP_ACC16,
  OP_ACC32,
  OP_ACC64,
};

/**
 * Definitions for the different macros responsible of checking
 * if the operand is a relative, register, immediate, memory, segment
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
#define op_seg(x) ((x) == OP_SEG_REG)
#define op_acc(x) ((x) <= OP_ACC64 && (x) >= OP_ACC8)

// --

#define op_byte(x) (x == OP_REL8 || x == OP_R8 || x == OP_IMM8 || x == OP_M8 || x == OP_ACC8)
#define op_word(x) (x == OP_REL16 || x == OP_R16 || x == OP_IMM16 || x == OP_M16 || x == OP_ACC16)
#define op_dword(x) (x == OP_REL32 || x == OP_R32 || x == OP_IMM32 || x == OP_M32 || x == OP_ACC32)
#define op_qword(x) (x == OP_R64 || x == OP_IMM64 || x == OP_M64 || x == OP_ACC64)

#define op_misc(x) (x == OP_SEG_REG || x == OP_NULL)

// --

#define op_rm(x) (op_r(x) || op_m(x))

/**
 * Type wrapper for an unsigned char that represents the
 * hash of an operand identifier. Used as a value for comparison
 * against the operand identity encoder lookup table. The individual
 * bits of the byte represent the type of operands within the
 * operand identifier as follows:
 *
 * 0: Relative operand (rel8/16/32/64)
 * 1: Register operand (r8/16/32/64)
 * 2: Immediate operand (imm8/16/32/64)
 * 3: Memory operand (m8/16/32/64)
 * 4: Segment register operand (seg_reg)
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
 * Function for hashing the operand identifier.
 * Used to compare against the operand identity encoder lookup table.
 * (Based on the operand types like REL, R, IMM, etc.)
 *
 * @param input The operand identifier in enum form
 * @return The operand hash value
 *
 * @see `op_ident_hash_t`
 * @see `op_ident_identify`
 *
 * @note returns 0b11111111 if the operand combinations
 * is not recognized
 */
op_ident_hash_t op_hash(enum operands input);

/**
 * Macro definitions for the different operand hash values.
 * Used to compare against the operand identity encoder lookup table.
 *
 * @see `op_ident_hash_t`
 *
 * ? Could be overcomplicated?
 */

#define OP_HASH_REL 0b00000001
#define OP_HASH_R 0b00000010
#define OP_HASH_IMM 0b00000100
#define OP_HASH_M 0b00001000
#define OP_HASH_SEG 0b00010000
#define OP_HASH_ACC 0b00100000

/**
 * Enumeration for the different operand identifiers.
 * Used to lookup the operand encoder functions.
 */
enum op_ident {
  OP_MR,
  OP_RM,
};

typedef struct {
  void *data;         /* Data in the operand */
  enum operands type; /* Type tied to the void pointer*/
} operand_t;

/**
 * Function for setting the prefix of the operand based on the
 * size of the reference operand (See below).
 *
 * @param buf The buffer to write the prefix to
 * @param op_arr The operand array to check the size of
 *
 * @note The reference operand shall not be confused with the
 * normal operand, the reference operand is the operand the
 * specific operand the instruction is based upon. On the contrary,
 * the normal operand is any other operand the instruction uses.
 *
 * 🤠
 */
void op_set_prefix(buffer_t *buf, const enum operands op);

/**
 * Function for identifying the operand identity, created using
 * a large c++ unordered_map.
 *
 * @param input The input operand list
 * @return The operand identity enumeration
 */
enum op_ident op_ident_identify(enum operands *input);

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

#endif

#ifdef __cplusplus
}
#endif
