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

// clang-format off

extern "C" {
  #include "operand.h"
  #include "encoder.h"
  #include "error.h"
  #include "instruction.h"
}

// clang-format on

#include "operand.hpp"
#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>

#define OP_HASH_NONE (uint32_t)0b11111111

static constexpr uint32_t __combine__(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
  return ((a) << 24 | (b) << 16 | (c) << 8 | (d));
}

static op_ident_hash_t op_hash(enum operands input) {
  if (op_rel(input)) return OP_HASH_REL;
  if (op_r(input)) return OP_HASH_R;
  if (op_imm(input)) return OP_HASH_IMM;
  if (op_m(input)) return OP_HASH_M;
  if (op_acc(input)) return OP_HASH_ACC;

  return OP_HASH_NONE;
}

namespace op {
  static std::unordered_map<uint32_t, enum enc_ident> lookup = {
      {__combine__(OP_HASH_R, OP_HASH_R, OP_HASH_NONE, OP_HASH_NONE), OP_MR},
      {__combine__(OP_HASH_M, OP_HASH_R, OP_HASH_NONE, OP_HASH_NONE), OP_MR},

      {__combine__(OP_HASH_R, OP_HASH_M, OP_HASH_NONE, OP_HASH_NONE), OP_RM},

      {__combine__(OP_HASH_R, OP_HASH_IMM, OP_HASH_NONE, OP_HASH_NONE), OP_MI},
      {__combine__(OP_HASH_M, OP_HASH_IMM, OP_HASH_NONE, OP_HASH_NONE), OP_MI},

      {__combine__(OP_HASH_NONE, OP_HASH_NONE, OP_HASH_NONE, OP_HASH_NONE), OP_ZO},
      {__combine__(OP_HASH_ACC, OP_HASH_IMM, OP_HASH_NONE, OP_HASH_NONE), OP_I},
      {__combine__(OP_HASH_REL, OP_HASH_NONE, OP_HASH_NONE, OP_HASH_NONE), OP_D},

      {__combine__(OP_HASH_R, OP_HASH_NONE, OP_HASH_NONE, OP_HASH_NONE), OP_M},
      {__combine__(OP_HASH_M, OP_HASH_NONE, OP_HASH_NONE, OP_HASH_NONE), OP_M},
  };
}
extern "C" enum enc_ident op_ident_identify(enum operands *input) {
  op_ident_hash_t hash[4];

  for (auto i = 0; i < 4; i++)
    hash[i] = op_hash(input[i]);

  uint32_t hash_key = 0;

  for (uint8_t i = 0; i < sizeof(hash); i++)
    hash_key |= (uint32_t)hash[i] << (24 - i * 8);

  if (op::lookup.find(hash_key) == op::lookup.end()) {
    err("Operand identifier not found.");
    return (enum enc_ident)0;
  }

  return (enum enc_ident)op::lookup[hash_key];
}
