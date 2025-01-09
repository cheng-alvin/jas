#include "error.h"
#include "operand.h"

static void pre_lea(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  if (op_m(op_arr[0].type) || op_r(op_arr[1].type))
    err("Invalid operands type for LEA instruction.");

  if (op_sizeof(op_arr[0].type) == 8)
    err("Byte operands cannot be used with the LEA instruction.");
}

static void no_operands(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  for (uint8_t i = 0; i < 4; i++) {
    if (op_arr[i].type != OP_NULL)
      err("This encoder identity does not support any operands.");
  }
}

static void same_operand_sizes(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  const uint8_t ref = op_sizeof(op_arr[0].type);

  for (uint8_t i = 0; i < 4; i++) {
    if (op_arr[i].type == OP_NULL) continue;

    if (op_sizeof(op_arr[i].type) != ref) {
      err("Invalid operand sizes.");
      break;
    }
  }
}

static void pre_imm(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  if (op_sizeof(op_arr[0].type) != 64) {
    same_operand_sizes(op_arr, buf, instr_ref, mode);
    return;
  }
}

static void pre_jcc_no_byte(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  if (op_sizeof(op_arr[0].type) == 8)
    err("Byte operands cannot be used with this instruction.");
}

static void pre_ret(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  if (op_sizeof(op_arr[0].type) != 16)
    err("Other operand sizes cannot be used with this instruction.");
}

static void pre_int(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  if (op_sizeof(op_arr[0].type) != 8)
    err("Invalid operand size for INT instruction.");
}

static void pre_small_operands(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  if (op_sizeof(op_arr[1].type) < 16)
    err("Invalid operand size for MOVZX/MOVSX instruction");
}