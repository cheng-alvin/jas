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

#include "operand.h"
#include "rex.h"
#include <stdint.h>

op_ident_hash_t op_hash(enum operands input) {
  if (op_rel(input))
    return OP_HASH_REL;

  if (op_r(input))
    return OP_HASH_R;

  if (op_imm(input))
    return OP_HASH_IMM;

  if (op_m(input))
    return OP_HASH_M;

  if (op_seg(input))
    return OP_HASH_SEG;

  if (op_acc(input))
    return OP_HASH_ACC;

  return 0b11111111;
}

uint8_t op_sizeof(enum operands input) {
  if (op_byte(input))
    return 8;

  if (op_word(input))
    return 16;

  if (op_dword(input))
    return 32;

  if (op_qword(input))
    return 64;

  return 0;
}

void op_set_prefix(buffer_t *buf, const enum operands op) {
  switch (op_sizeof(op)) {
  case 8:
    buf_write(buf, 0x0F, 1);
    break;

  case 16:
    buf_write(buf, OP_WORD_OVERRIDE, 1);
    break;

  case 32:
    break;

  case 64:
    buf_write(buf, REX_W, 1);
    break;

  default:
    err("Invalid operand size.");
    return;
  }
}