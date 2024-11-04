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

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "encoder.h"
#include "mode.h"
#include "operand.h"
#include <stdbool.h>
#include <stdint.h>

// Forward declaration - see instr_encode_table
typedef struct instr_encode_table instr_encode_table_t;

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

  // Directives:
  INSTR_DIR_LOCAL_LABEL,
  INSTR_DIR_GLOBAL_LABEL,
  INSTR_DIR_EXTERN_LABEL,
  INSTR_DIR_WRT_BUF, // A buffer_t to write to
};

/**
 * Type wrapper for the instruction encoder function pointer. Where
 * each operand encoder function takes an array of operands and
 * a buffer to write the encoded instruction to.
 *
 * (Based on the operand identities like MR, RM, etc.)
 *
 * @param op_arr The array of operands to encode
 * @param buf The buffer to write the encoded instruction to
 * @param instr_ref The instruction reference table
 * @param mode The operating mode of the instruction
 *
 * @note All encoder functions will conform to this signature.
 */
typedef void (*instr_encoder_t)(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode);

struct instr_encode_table {
  enum enc_ident ident;         /* Operand encoding identity */
  uint8_t opcode_ext;           /* Opcode extension */
  uint8_t opcode[3];            /* Opcode of the instruction */
  mode_support_t support;       /* Support status of the instruction (Optional, Would be set to "all" if not used) */
  uint8_t byte_instr_opcode[3]; /* 8 bit opcode fallback of the instruction */
  uint8_t opcode_size;          /* Size of the opcode (max. 3 bytes)*/
  instr_encoder_t pre;          /* Pre-encoder processor function (Optional, null if not applicable) */
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

/**
 * Lookup table for the different instruction class encoders.
 *
 * @param input The instruction encoding identity
 * @return The instruction encoder function pointer
 */
instr_encoder_t instr_encode_func(enum enc_ident input);

#define INSTR_TERMINATOR         \
  (instr_encode_table_t) {       \
    .ident = NULL,               \
    .opcode_ext = NULL,          \
    .opcode = {NULL},            \
    .support = MODE_SUPPORT_ALL, \
    .byte_instr_opcode = {NULL}, \
    .opcode_size = NULL,         \
    .pre = NULL,                 \
  }

#define INSTR_NULL \
  (instruction_t) { .instr = NULL, .operands = NULL }

#endif
