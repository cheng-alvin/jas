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

// TODO Main encoder function to be added soon.
#endif