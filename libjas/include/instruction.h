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

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "encoder.h"
#include <stdbool.h>
#include <stdint.h>

// Forward declaration - see instr_encode_table
typedef struct instr_encode_table instr_encode_table_t;

#include "dir.h"

enum instructions {
  INSTR_NULL,

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
  INSTR_MOVZX,
  INSTR_MOVSX,
  INSTR_XCHG,
  INSTR_BSWAP,
  INSTR_CMOVA,
  INSTR_CMOVAE,
  INSTR_CMOVB,
  INSTR_CMOVBE,
  INSTR_CMOVE,
  INSTR_CMOVG,
  INSTR_CMOVGE,
  INSTR_CMOVL,
  INSTR_CMOVLE,
  INSTR_CMOVNA,
  INSTR_CMOVNAE,
  INSTR_CMOVNB,
  INSTR_CMOVNBE,
  INSTR_CMOVNE,
  INSTR_CMOVNG,
  INSTR_CMOVNGE,
  INSTR_CMOVNL,
  INSTR_CMOVNLE,
  INSTR_CMOVNO,
  INSTR_CMOVNP,
  INSTR_CMOVNS,
  INSTR_CMOVNZ,
  INSTR_CMOVO,
  INSTR_CMOVP,
  INSTR_CMOVPE,
  INSTR_CMOVPO,
  INSTR_CMOVS,
  INSTR_CMOVZ,
};

// Alias type for the encoder `encoder_t` function pointer. - See `encoder.h`
typedef encoder_t pre_encoder_t;

struct instr_encode_table {
  enum enc_ident ident;         /* Operand encoding identity */
  uint8_t opcode_ext;           /* Opcode extension */
  uint8_t opcode[3];            /* Opcode of the instruction */
  uint8_t byte_instr_opcode[3]; /* 8 bit opcode fallback of the instruction */
  uint8_t opcode_size;          /* Size of the opcode (max. 3 bytes)*/
  uint8_t byte_opcode_size;     /* Size of the byte opcode (max. 3 bytes, may be left null) */
};

/**
 * The lookup table using the `instructions_t` enum as the index
 * to get the corresponding instruction operand encoder structs.
 */
extern instr_encode_table_t *instr_table[];

typedef struct instruction {
  enum instructions instr; /* Type of instruction */
  operand_t *operands;     /* Operands of the instruction */
} instruction_t;

typedef struct instr_generic {
  enum { INSTR,
         DIRECTIVE } type; /* Type of assembler input */

  /**
   * Two forms of inputs are accepted into the Jas assembler, an
   * instruction and directive. Directives are used to alter the
   * basic behavior of the assembler and its compilation steps.
   *
   * The union shares memory with both functionality, allowing
   * both types of data to be read through a single structure.
   */
  union {
    struct instruction instr;
    struct directive dir;
  };
} instr_generic_t;

#define INSTR_TAB_NULL           \
  (instr_encode_table_t) {       \
    .ident = NULL,               \
    .opcode_ext = NULL,          \
    .opcode = {NULL},            \
    .byte_instr_opcode = {NULL}, \
    .opcode_size = NULL,         \
    .byte_opcode_size = NULL,    \
  }

/**
 * Function for getting the instruction table based on the instruction
 * struct provided. The function will return a instruction table struct
 * as described above in this header file.
 *
 * @param instr The instruction struct to get the identifier from
 * @return The instruction table struct
 *
 * @see `instr_encode_table_t`
 * @see `instruction_t`
 */
instr_encode_table_t instr_get_tab(instruction_t instr);

/**
 * Function used to generate an instruction directive, representing a
 * Jas buffer for the manual addition of raw binary bytes during the
 * compilation process, akin to the 'db' directive in NASM.
 *
 * @param data_sz Total size of data to write.
 * @param ... Individual byte values to append in `uint8_t` form.
 *
 * @return *Allocated* instruction directive pointer.
 */
instr_generic_t *instr_write_bytes(size_t data_sz, ...);

// Helper macro definitions in lieu of `instr_gen` functions:

#define r64(x) OP_R64, x, 0
#define r32(x) OP_R32, x, 0
#define r16(x) OP_R16, x, 0
#define r8(x) OP_R8, x, 0

// --

#define imm8(x) OP_IMM8, x, 0
#define imm16(x) OP_IMM16, x, 0
#define imm32(x) OP_IMM32, x, 0
#define imm64(x) OP_IMM64, x, 0

/**
 * These Relative macros are used to define the relative operand,
 * usually using labels. The relative operand is used in jump
 * instructions and other instructions that require a relative
 * offset to a label.
 *
 * @example rel("label", 10)
 */

#define rel8(x, off) OP_REL8, x, off
#define rel32(x, off) OP_REL32, x, off

// Note: offset must be provided - equivalent to: [eax + xyz]
// (SIB bytes and another register for displacement not supported)

#define m8(x, off) OP_M8, x, off
#define m16(x, off) OP_M16, x, off
#define m32(x, off) OP_M32, x, off
#define m64(x, off) OP_M64, x, off

/**
 * A function for easily defining a instruction in the `instruction_t`
 * form without having to use the struct initializer or mangle around
 * with void pointers and curly braces. This function can create
 * and automatically allocate an `instruction` struct using variadic
 * arguments.
 *
 * @param instr The instruction type, as defined in `enum instructions`
 * @param operand_count The number of operands to pass (max. 4)
 *
 * @param ... The operands to pass
 * @note Can be used in conjunction with the helper macros defined
 * above to improve readability, allowing code to be "self-documenting".
 *
 * @return An allocated `instr_generic` struct pointer.
 */
instr_generic_t *instr_gen(enum instructions instr, uint8_t operand_count, ...);

/**
 * Helper macro for freeing an array of instruction structs and
 * their operands. The macro is used to free the memory allocated
 * using the help of the `instr_free()` function `free()`
 *
 * @param instr_arr The array of instruction structs
 * @param arr_size The size of the instruction array
 *
 * @note If the instruction array is dynamically allocated, the
 * memory allocated for the array *itself* must be taken care of
 * by the caller instead of this function/macro.
 */
#define instr_free_all(instr_arr, arr_size) \
  do {                                      \
    for (size_t i = 0; i < arr_size; i++)   \
      instr_free(instr_arr[i]);             \
  } while (0)

/**
 * Function for freeing memory allocated through internal Jas instruction
 * generation functions. All subsequent nested allocations would also be
 * freed via this function.
 *
 * @param instr The instruction generic struct to free
 *
 * @note To prevent memory leaks and undefined behavior associated with
 * the repeated freeing of memory, the pointer used must only be alloc-
 * ated through Jas internal functions.
 */
void instr_free(instr_generic_t *instr);

#endif
