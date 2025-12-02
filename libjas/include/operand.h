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
#include "register.h"
#include "rex.h"
#include <stdint.h>

typedef struct instr_encode_table instr_encode_table_t;

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

/**
 * Function for the determining the mode of the ModR/M byte through
 * a given displacement and corresponding displacement size pointer.
 * This function also supports the determination of the size of the
 * displacement value, such behavior may be inhibited by passing `NULL`
 *
 * @param displacement The value of the displaced value.
 * @param sz Pointer representing the size of the displacement.
 *
 * @return A mode applicable to the ModR/M byte of provided displacement.
 */
enum op_modrm_modes op_modrm_mode(uint64_t displacement, uint8_t *sz);

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
  OP_NULL = 0,

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

enum op_sib_scale {
  OP_SIB_SCALE_1 = 0,
  OP_SIB_SCALE_2 = 1,
  OP_SIB_SCALE_4 = 2,
  OP_SIB_SCALE_8 = 3,
};

/// @brief Union for representing memory source operands, rather than the
/// the use of a direct value itself. It can be thought of as being directed
/// towards a memory address through [], instead of directly.

/// @note The `label` member must not be confused with the generic `label`.

typedef union op_mem_src {
  char *label; /* String named reference to a label */
  struct {
    enum op_sib_scale scale; /* Scale that should be applied */

    /// @note `reg` should be used as primary register, rather than
    /// `reg_disp`. Said member should only be used in line with an
    /// SIB instance.

    enum registers reg;
    enum registers reg_disp;
  } sib; /* Standard memory-block model */
} op_mem_src_t;

/// @brief Abstracted encapsulation of a memory operand, allowing for added
/// portability and readability when dealing with instruction creation.
typedef struct op_mem {
  enum { LABEL,
         SIB } src_type; // Source type selector enumeration

  union op_mem_src src; // Source of memory operand, refer to `op_mem_src`
  uint64_t disp;        // RIP addressing requires 32-bit displacement
} op_mem_t;

typedef struct operand {
  enum operands type; /* Type describing data stored */

  /**
   * The operand struct can be used for the representation of
   * a range of operand types, with a range of operand sizes.
   * The following union is used to represent data, which is
   * in form as indicated by the `type` enumerated value.
   *
   * In lieu of providing a reference to a type's specific value
   * through a void pointer, the usage of a union allows for
   * the improved readability for the Jas codebase.
   */
  union {
    uint64_t imm; /* Immediate value, dependent on `type` */
    char *label;

    /// @note The encoding of the `mem` element as an SIB byte,
    /// ModR/M or RIP addressing is of sole decision of the encoder
    /// function. Not be confused with `op_sib`
    op_mem_t mem;
  };

} operand_t;

/// Implementation @note: when ModR/M's R/M field is 0b100 (4), an SIB
/// would be expected to be present after the ModR/M byte. A placeholder
/// such as 0x24 can be used in lieu of an actual SIB byte where applicable.

typedef struct __attribute__((packed)) op_sib {
  enum op_sib_scale scale : 2; /* Scale factor for index register */

  // Note how scale applies to index and not the base, hence expression:
  // base + (index * scale) [ + displacement, where applicable]

  uint8_t index : 3; /* Index register */
  uint8_t base : 3;  /* Base register */
} op_sib_t;

/**
 * Function for setting the prefix of the operand based on the
 * size of the reference operand array, and writes it the
 * buffer provided in `buf`.
 *
 * @param buf The pointer to the buffer to write the prefixes to.
 * @param op_arr The operand array to base the prefixes from.
 * @param mode The current operating mode of the instruction
 */
void op_write_prefix(buffer_t *buf, operand_t *op_arr, enum modes mode);

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
 * Function for asserting that the types of the input operand
 * array matches the expected operand types provided in the
 * expected operand array.
 *
 * @param in The input operand array to check against `ex`
 * @param ex List of expected operand types in enumerated form.
 *
 * @param sz The size of the arrays provided.
 *
 * @return The result of the assertion; true if all types match
 * with the expected types, false otherwise.
 */
bool op_assert_types(operand_t *in, enum operands *ex, size_t sz);

#endif
