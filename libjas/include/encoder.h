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

#ifndef ENCODER_H
#define ENCODER_H

#include "operand.h"
#include <stdint.h>

/**
 * Enum to represent the different encoder identities, gives
 * guidance on which corresponding field can be filled in with
 * the instruction encoder table.
 *
 * @note See identities and their corresponding encoder
 * methods in the main source file - `encoder.c`.
 */
enum enc_ident {
  /**
   * Note, enumeration values from 0 to 7 are reserved for use
   * for representation of the ModR/M byte's operand extension.
   *
   * Where applicable, the encoding reference table can request
   * the usage of opcode extensions such as `/0` and can be assigned
   * an identity of `(enum enc_ident)0` to `(enum enc_ident)7`,
   * representing `/0` to `/7` respectively.
   */
  ENC_NULL = 8,

  ENC_REG,
  ENC_RM, // Would be determined automatically, adding SIB where applicable.
  ENC_IMM,
  ENC_REL,
  ENC_OPCODE_APPENDED,
};

typedef struct enc_serialized_instr {
  buffer_t prefixes; /* Instruction prefixes (raw bytes) */

  /* Opcode */
  uint8_t opcode[3];       /* Opcode bytes (max 3) */
  uint8_t opcode_size : 2; /* Size of opcode (0–3 bytes) */

  /* ModR/M + SIB -  Memory representation */
  struct op_modrm modrm; /* ModR/M byte */
  struct op_sib sib;     /* SIB byte */
  bool has_modrm : 1;    /* ModR/M present */
  bool has_sib : 1;      /* SIB present */

  uint64_t disp; /* Displacement value (Little endian) */
  uint64_t imm;  /* Immediate value  (Little endian) */

  /// @note A 0 size field depicts absence of the respective field.
  uint8_t disp_size : 4; /* Displacement size (0–8 bytes) */
  uint8_t imm_size : 4;  /* Immediate size (0–8 bytes) */

} enc_serialized_instr_t;

#endif