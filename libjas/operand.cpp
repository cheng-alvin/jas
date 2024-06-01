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
#include "error.h"
#include "operand.hpp"
#include <cstddef>
#include <cstdint>
#include <unordered_map>

static constexpr uint32_t __combine__(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
  return ((a) << 24 | (b) << 16 | (c) << 8 | (d));
}

static op_ident_hash_t op_hash(enum operands input) {
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

namespace op {
  static std::unordered_map<uint32_t, enum op_ident> lookup = {
      {__combine__(OP_HASH_R, OP_HASH_R, NULL, NULL), OP_MR},
      {__combine__(OP_HASH_M, OP_HASH_R, NULL, NULL), OP_MR},

      {__combine__(OP_HASH_R, OP_HASH_R, NULL, NULL), OP_RM},
      {__combine__(OP_HASH_R, OP_HASH_M, NULL, NULL), OP_RM},
  };
}

extern "C" enum op_ident op_ident_identify(enum operands *input) {
  op_ident_hash_t hash[4];

  for (auto i = 0; i < sizeof(input); i++)
    hash[i] = op_hash(input[i]);

  uint32_t hash_key = 0;

  for (uint8_t i = 0; i < sizeof(hash); i++)
    hash_key |= ((uint32_t)hash[i]) << (24 - i * 8);

  if (op::lookup.find(hash_key) == op::lookup.end()) {
    err("Operand identifier not found.");
    return;
  }

  return (enum op_ident)op::lookup[hash_key];
}
