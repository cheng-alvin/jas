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

#define OP_REL(x) ((x) <= OP_REL32 && (x) >= OP_REL8)
#define OP_R(x) ((x) <= OP_R64 && (x) >= OP_R8)
#define OP_IMM(x) ((x) <= OP_IMM64 && (x) >= OP_IMM8)
#define OP_M(x) ((x) <= OP_M64 && (x) >= OP_M8)
#define OP_MEM(x) ((x) <= OP_MEM64 && (x) >= OP_MEM)
#define OP_SEG(x) ((x) == OP_SEG_REG)
#define OP_ACC(x) ((x) <= OP_ACC64 && (x) >= OP_ACC8)
#define OP_RM(x) (OP_R(x) || OP_M(x)

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

#endif

#ifdef __cplusplus
}
#endif
