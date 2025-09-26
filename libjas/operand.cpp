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
#include <map>

#define OP_HASH_NONE (uint32_t)0b11111111

static constexpr uint32_t __combine__(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
  return ((a) << 24 | (b) << 16 | (c) << 8 | (d));
}

static op_ident_hash_t op_hash(enum operands input) {
  if (op_rel(input)) return OP_HASH_REL;
  if (op_r(input)) return OP_HASH_R;
  if (op_imm(input)) return OP_HASH_IMM;
  if (op_m(input)) return OP_HASH_M;

  return OP_HASH_NONE;
}

static multimap<enum enc_ident, uint32_t> master = {
    {ENC_MR, __combine__(OP_HASH_R, OP_HASH_R, OP_HASH_NONE, OP_HASH_NONE)},
    {ENC_MR, __combine__(OP_HASH_M, OP_HASH_R, OP_HASH_NONE, OP_HASH_NONE)},

    {ENC_RM, __combine__(OP_HASH_R, OP_HASH_M, OP_HASH_NONE, OP_HASH_NONE)},
    {ENC_RM, __combine__(OP_HASH_M, OP_HASH_R, OP_HASH_NONE, OP_HASH_NONE)},

    {ENC_MI, __combine__(OP_HASH_R, OP_HASH_IMM, OP_HASH_NONE, OP_HASH_NONE)},
    {ENC_MI, __combine__(OP_HASH_M, OP_HASH_IMM, OP_HASH_NONE, OP_HASH_NONE)},

    {ENC_ZO, __combine__(OP_HASH_NONE, OP_HASH_NONE, OP_HASH_NONE, OP_HASH_NONE)},
    {ENC_I, __combine__(OP_HASH_IMM, OP_HASH_NONE, OP_HASH_NONE, OP_HASH_NONE)},
    {ENC_D, __combine__(OP_HASH_REL, OP_HASH_NONE, OP_HASH_NONE, OP_HASH_NONE)},

    {ENC_M, __combine__(OP_HASH_R, OP_HASH_NONE, OP_HASH_NONE, OP_HASH_NONE)},
    {ENC_M, __combine__(OP_HASH_M, OP_HASH_NONE, OP_HASH_NONE, OP_HASH_NONE)},
};

static bool ident_exists(enum enc_ident ident, instr_encode_table_t *instr_ref) {
  for (auto j = 0; instr_ref[j].opcode_size > 0; j++)
    if (instr_ref[j].ident == ident) return true;

  return false;
}

extern "C" enum enc_ident op_ident_identify(enum operands *input, instr_encode_table_t *instr_ref) {
  uint32_t hash_key = 0;

  for (uint8_t i = 0; i < 4; i++)
    hash_key |= (uint32_t)op_hash(input[i]) << (24 - i * 8);

  auto j = 0;
  multimap<uint32_t, enum enc_ident> lookup_table;
  while (instr_ref[j].opcode_size) {
    auto range = master.equal_range(instr_ref[j].ident);
    for (auto it = range.first; it != range.second; it++) {
      lookup_table.insert({it->second, instr_ref[j].ident});
    }
    j++;
  }

  if (lookup_table.find(hash_key) != lookup_table.end()) {
    enum enc_ident target = lookup_table.find(hash_key)->second;

    bool o_exists = ident_exists(ENC_O, instr_ref);
    bool oi_exists = ident_exists(ENC_OI, instr_ref);

    if (o_exists || oi_exists) {
      const uint32_t acc_hash = __combine__(OP_HASH_R, OP_HASH_IMM, OP_HASH_NONE, OP_HASH_NONE);

      // Spaghetti code, would be ultimately removed
      if (hash_key == acc_hash && o_exists) return ENC_O;
      return ENC_OI;
    }

    return target;
  } else {
    uint8_t k = 0;
    while (instr_ref[k].opcode_size) {
      if (instr_ref[k].ident == ENC_IGN) return ENC_IGN;
      k++;
    } /* Fall though: */
    err("No corresponding instruction opcode found.");
    return ENC_NULL;
  }
}
