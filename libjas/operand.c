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
#include "mr.h"
#include "rm.h"
#include <stddef.h>

op_encoder_t op_encode_func(enum op_ident input) {
  op_encoder_t lookup[] = {&mr, &rm};
  return lookup[(size_t)input];
}

op_ident_hash_t op_ident_hash(enum operands input) {
  if (OP_REL(input))
    return OP_HASH_REL;

  if (OP_R(input))
    return OP_HASH_R;

  if (OP_IMM(input))
    return OP_HASH_IMM;

  if (OP_M(input))
    return OP_HASH_M;

  if (OP_SEG(input))
    return OP_HASH_SEG;

  if (OP_ACC(input))
    return OP_HASH_ACC;

  return 0b11111111;
}