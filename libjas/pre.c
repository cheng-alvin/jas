#include "error.h"
#include "operand.h"

#define DEFINE_PRE_ENCODER(name) \
  static void name(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode)

DEFINE_PRE_ENCODER(pre_lea) {
  if (op_m(op_arr[0].type) || op_r(op_arr[1].type))
    err("Invalid operands type for LEA instruction.");

  if (op_sizeof(op_arr[0].type) == 8)
    err("Byte operands cannot be used with the LEA instruction.");
}

DEFINE_PRE_ENCODER(no_operands) {
  for (uint8_t i = 0; i < 4; i++) {
    if (op_arr[i].type != OP_NULL)
      err("This encoder identity does not support any operands.");
  }
}

DEFINE_PRE_ENCODER(same_operand_sizes) {
  const uint8_t ref = op_sizeof(op_arr[0].type);

  for (uint8_t i = 0; i < 4; i++) {
    if (op_arr[i].type == OP_NULL) continue;

    if (op_sizeof(op_arr[i].type) != ref) {
      err("Invalid operand sizes.");
      break;
    }
  }
}

DEFINE_PRE_ENCODER(pre_imm) {
  if (op_sizeof(op_arr[0].type) != 64) {
    same_operand_sizes(op_arr, buf, instr_ref, mode);
    return;
  }
}

DEFINE_PRE_ENCODER(pre_jcc_no_byte) {
  if (op_sizeof(op_arr[0].type) == 8)
    err("Byte operands cannot be used with this instruction.");
}

DEFINE_PRE_ENCODER(pre_ret) {
  if (op_sizeof(op_arr[0].type) != 16)
    err("Other operand sizes cannot be used with this instruction.");
}

DEFINE_PRE_ENCODER(pre_int) {
  if (op_sizeof(op_arr[0].type) != 8)
    err("Invalid operand size for INT instruction.");
}

DEFINE_PRE_ENCODER(pre_small_operands) {
  if (op_sizeof(op_arr[1].type) < 16)
    err("Invalid operand size for MOVZX/MOVSX instruction");
}