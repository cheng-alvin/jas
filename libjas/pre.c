#include "error.h"
#include "instruction.h"
#include "mode.h"
#include "operand.h"
#include "register.h"

// Notes on refactoring of these:
// - Only MI and I encoders need to have different operand sizes.
// The rules for previous `same_operand_sizes` can be applied to
// MR and similar encoders, check `mr_rm_common`.
//

#define DEFINE_PRE_ENCODER(name) \
  static void name(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode)

DEFINE_PRE_ENCODER(pre_ret) {
  if (op_sizeof(op_arr[0].type) != 16)
    err("Other operand sizes cannot be used with this instruction.");
}

DEFINE_PRE_ENCODER(pre_small_operands) {
  if (op_sizeof(op_arr[1].type) < 16)
    err("Invalid operand size for MOVZX/MOVSX instruction");
}
