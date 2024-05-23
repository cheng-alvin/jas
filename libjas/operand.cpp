#include "operand.h"
#include "null.h"
#include <cstdint>
#include <unordered_map>

/**
 * Type wrapper for an unsigned char that represents the
 * hash of an operand identifier. Used as a value for comparison
 * against the operand identity encoder lookup table. The individual
 * bits of the byte represent the type of operands within the
 * operand identifier as follows:
 *
 * 0: Relative operand (rel8/16/32/64)
 * 1: Register operand (r8/16/32/64)
 * 2: Immediate operand (imm8/16/32/64)
 * 3: Memory operand (m8/16/32/64)
 * 4: Segment register operand (seg_reg)
 * 5: Accumulator register operand (acc8/16/32/64)
 *
 * 6-7: Reserved for future use
 *
 * A bit set to 1 indicates that the operand is of the corresponding
 * type, while a bit set to 0 indicates that the operand is not of the
 * corresponding type.
 *
 * @note Not to be confused with the operand types (`enum operands`)
 *
 * Also, there are macros below to help you!
 */
typedef uint8_t op_ident_hash_t;

/**
 * Macro definitions for the different operand hash values.
 * Used to compare against the operand identity encoder lookup table.
 *
 * @see `op_ident_hash_t`
 *
 * ? Could be overcomplicated?
 */

#define OP_HASH_REL 0b00000001
#define OP_HASH_R 0b00000010
#define OP_HASH_IMM 0b00000100
#define OP_HASH_M 0b00001000
#define OP_HASH_SEG 0b00010000
#define OP_HASH_ACC 0b00100000

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

  return (enum op_ident)op::lookup[hash_key];
}
