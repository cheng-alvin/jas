#include "operand.h"
#include "null.h"
#include <cstdint>
#include <unordered_map>

// TODO Refactor file for future reference
#define COMBINE_HASHES(a, b, c, d) ((a) << 24 | (b) << 16 | (c) << 8 | (d))

namespace op {
  static std::unordered_map<uint32_t, enum op_ident> lookup = {
      {COMBINE_HASHES(OP_HASH_R, OP_HASH_R, NULL, NULL), OP_MR},
      {COMBINE_HASHES(OP_HASH_M, OP_HASH_R, NULL, NULL), OP_MR},

      {COMBINE_HASHES(OP_HASH_R, OP_HASH_R, NULL, NULL), OP_RM},
      {COMBINE_HASHES(OP_HASH_R, OP_HASH_M, NULL, NULL), OP_RM},
  };
}

extern "C" enum op_ident op_ident_identify(enum operands *input) {
  // TODO Questionable code, please do remember to refactor 😌
  op_ident_hash_t hash[4] = {0};

  for (auto i = 0; i < sizeof(input); i++)
    hash[i] = op_hash(input[i]);

  uint32_t hash_key = 0;

  // TODO Fix this ugly hack
  for (auto i = 0; i < sizeof(hash); i++)
    hash_key |= hash[i] << (24 - i * 8); /* ? Very questionable */

  return (enum op_ident)op::lookup[hash_key];
}
