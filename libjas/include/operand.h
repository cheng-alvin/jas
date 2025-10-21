/**
 * MIT License
 * Copyright (c) 2023-2025 Alvin Cheng <eventide1029@gmail.com>
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
#include "codegen.h"
#include "encoder.h"
#include "mode.h"
#include <stdint.h>

/**
 * Macro definition for the 16-bit operand override byte for supporting
 * word-sized operands and addresses in the x86 family.
 *
 * @see https://stackoverflow.com/questions/74954166/
 */
#define OP_WORD_OVERRIDE 0x66
#define OP_ADDR_OVERRIDE 0x67

/**
 * Struct definition for the ModR/M byte used in x86 instruction
 * encoding for specifying operands, especially for register or
 * memory-related operands.
 *
 * The struct is packed to preserve exact bit layout as expected in
 * encoding. Hence, The struct is designed to be casted and written
 * directly.
 */

enum op_modrm_modes {
  OP_MODRM_MODE_INDIRECT = 0,
  OP_MODRM_MODE_DISP8 = 1,
  OP_MODRM_MODE_DISP32 = 2,
  OP_MODRM_MODE_REG = 3,
};

typedef struct __attribute__((packed)) op_modrm {
  enum op_modrm_modes mod : 2; /* Mode selection of ModR/M operand */

  uint8_t reg : 3; /* Register field, or for an extended opcode */
  uint8_t rm : 3;  /* Memory or additional register fields */
} op_modrm_t;

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

// --

#define op_byte(x) (x == OP_REL8 || x == OP_R8 || x == OP_IMM8 || x == OP_M8)
#define op_word(x) (x == OP_REL16 || x == OP_R16 || x == OP_IMM16 || x == OP_M16)
#define op_dword(x) (x == OP_REL32 || x == OP_R32 || x == OP_IMM32 || x == OP_M32)
#define op_qword(x) (x == OP_R64 || x == OP_IMM64 || x == OP_M64)

// --

#define op_rm(x) (op_r(x) || op_m(x))

typedef struct operand {
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
enum enc_ident op_ident_identify(enum operands *input, instr_encode_table_t *instr_ref);

/**
 * Simple function for determining the ModR/M mode based on the
 * operand type and offset provided by the operand struct.
 * (As defined below)
 *
 * @param input The operand struct to determine the mode from
 * @return The ModR/M mode
 *
 * @note Function also performs checks for RIP, ESP, IP instr-
 * uction pointers for offset and ModR/M bytes and modes.
 *
 * @note The function requires the offset value to be typed as
 * signed types to prevent confusion and to match with the Intel
 * -required specifications as outlined.
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
 * Function for returning the opcode of the instruction based
 * on the instruction encoder table provided in the function
 * arguments as well as if a byte opcode is provided in the
 * the encoder table.
 *
 * @param op_arr The operand array to base the opcode from
 * @param instr_ref The instruction reference table
 * @return The opcode of the instruction
 *
 * You can literally wrap this into a buffer write function
 * to quickly and easily write in the opcode like as shown:
 *
 *  buf_write(buf, op_write_opcode(<operands>, <encoder table>), <opcode size>);
 *
 * @see buffer.h
 */
uint8_t *op_write_opcode(operand_t *op_arr, instr_encode_table_t *instr_ref);

#endif
