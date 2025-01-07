/**
 * MIT License
 * Copyright (c) 2023-2024 Alvin Cheng <eventide1029@gmail.com>
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
#include "mode.h"
#include "operand.h"
#include <stdbool.h>
#include <stdint.h>

// Forward declaration - see instr_encode_table
typedef struct instr_encode_table instr_encode_table_t;

/**
 * Lookup macro for checking if an instruction is a directive.
 * Directives are instructions that are not executed by the CPU
 * but are used to provide information to the assembler.
 *
 * @param i The instruction to check
 * @example if (INSTR_DIRECTIVE(INSTR_MOV))
 *
 * @note - Internal
 * Please update this macro if more directives and/or instructions
 * are added/supported in this enum list (In the future).
 */
#define INSTR_DIRECTIVE(i) ((uint8_t)i > (uint8_t)INSTR_DUMMY)

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

  INSTR_DUMMY,

  // Directives:
  INSTR_DIR_WRT_BUF, // A buffer_t to write to

  INSTR_DIR_LOCAL_LABEL,
  INSTR_DIR_GLOBAL_LABEL,
  INSTR_DIR_EXTERN_LABEL,
};

// Alias type for the encoder `encoder_t` function pointer. - See `encoder.h`
typedef encoder_t pre_encoder_t;

struct instr_encode_table {
  enum enc_ident ident;         /* Operand encoding identity */
  uint8_t opcode_ext;           /* Opcode extension */
  uint8_t opcode[3];            /* Opcode of the instruction */
  mode_support_t support;       /* Support status of the instruction (Optional, Would be set to "all" if not used) */
  uint8_t byte_instr_opcode[3]; /* 8 bit opcode fallback of the instruction */
  uint8_t opcode_size;          /* Size of the opcode (max. 3 bytes)*/
  pre_encoder_t pre;            /* Pre-encoder processor function (Optional, null if not applicable) */
  bool has_byte_opcode;         /* If the instruction encoder table has a byte instruction opcode */
};

/**
 * The lookup table using the `instructions_t` enum as the index
 * to get the corresponding instruction operand encoder structs.
 */
extern instr_encode_table_t *instr_table[];

typedef struct {
  enum instructions instr; /* Type of instruction */
  operand_t *operands;     /* Operands of the instruction */
} instruction_t;

#define INSTR_TERMINATOR         \
  (instr_encode_table_t) {       \
    .ident = NULL,               \
    .opcode_ext = NULL,          \
    .opcode = {NULL},            \
    .support = MODE_SUPPORT_ALL, \
    .byte_instr_opcode = {NULL}, \
    .opcode_size = NULL,         \
    .pre = NULL,                 \
    .has_byte_opcode = false     \
  }

#define INSTR_NULL \
  (instruction_t) { .instr = NULL, .operands = NULL }

// Macro for checking if the instruction is a label and shall be handled
#define IS_LABEL(x)                                      \
  (uint8_t) x.instr >= (uint8_t)INSTR_DIR_LOCAL_LABEL && \
      (uint8_t)x.instr <= (uint8_t)INSTR_DIR_EXTERN_LABEL

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
 * A function for easily defining a instruction in the `instruction_t`
 * form without having to use the struct initializer or mangle around
 * with void pointers and curly braces. This function is used to create
 * a instruction struct and its operands using variadic arguments
 * and grouping 3 arguments together and passed off into the function.
 *
 * @param instr The instruction type
 * @param operand_count The number of operands to pass
 * @param ... The operands to pass (Refer to below example)
 *
 * @return The instruction struct
 *
 * @note All operands will be grouped into three arguments, a type, offset
 * and data, similar to the ones of the `operand_t` struct.
 *
 * @example instr_gen(INSTR_XXX, 1, OP_R64, REG_RAX, 0);
 *
 * The example above will generate a instruction struct with the
 * instruction type `INSTR_XXX` and a single operand with the type
 * `OP_R64`, an offset of `0` and the data `REG_RAX`, which returns:
 *
 * ```
 * (instruction_t){
 *   .instr = INSTR_XXX,
 *   .operands = (operand_t[]){
 *     (operand_t)
 *     {.type = OP_R64, .offset = 0, .data = &(enum registers){REG_RAX}},
 *     OP_NONE, OP_NONE, OP_NONE,
 *   },
 * }
 *```
 *
 * @see `operand_t`
 */
instruction_t instr_gen(enum instructions instr, uint8_t operand_count, ...);

#endif
