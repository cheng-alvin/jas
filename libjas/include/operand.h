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
 */

#define op_rel(x) ((x) <= op_rel32 && (x) >= op_rel8)
#define op_r(x) ((x) <= op_r64 && (x) >= op_r8)
#define op_imm(x) ((x) <= op_imm64 && (x) >= op_imm8)
#define op_m(x) ((x) <= op_m64 && (x) >= op_m8)
#define op_seg(x) ((x) == op_seg_REG)
#define op_acc(x) ((x) <= op_acc64 && (x) >= op_acc8)

#define
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
 * Type wrapper for the operand encoder function pointer. Where
 * each operand encoder function takes an array of operands and
 * a buffer to write the encoded instruction to.
 *
 * (Based on the operand identities like MR, RM, etc.)
 *
 * @param op_arr The array of operands to encode
 * @param buf The buffer to write the encoded instruction to
 * @param instr_ref The instruction reference table
 *
 * @note All encoder functions will conform to this signature.
 */
typedef void (*op_encoder_t)(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref);

/**
 * Lookup table for the different operand encoders.
 *
 * @param input The operand identifier
 * @return The operand encoder function pointer
 */
op_encoder_t op_encode_func(enum op_ident input);

/**
 * Function for identifying the operand identity, created using
 * a large c++ unordered_map.
 *
 * @param input The input operand list
 * @return The operand identity enumeration
 */
enum op_ident op_ident_identify(enum operands *input);

#endif

#ifdef __cplusplus
}
#endif
