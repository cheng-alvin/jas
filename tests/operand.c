#include "operand.h"
#include "buffer.h"
#include "encoder.h"
#include "instruction.h"
#include "rex.h"
#include "test.h"

Test(operand, write_prefix) {
  buffer_t buf = BUF_NULL;

  const operand_t op_arr16[] = {r16, imm16, OP_NONE, OP_NONE};
  const operand_t op_addr16[] = {m16, imm16, OP_NONE, OP_NONE};
  const operand_t op_arr32[] = {r32, imm32, OP_NONE, OP_NONE};
  const operand_t op_addr32[] = {m32, imm32, OP_NONE, OP_NONE};
  const operand_t op_arr64[] = {r64, imm64, OP_NONE, OP_NONE};
  const operand_t op_addr64[] = {m64, imm64, OP_NONE, OP_NONE};

#define RUN_TEST(operands, mode, index, expected) \
  op_write_prefix(&buf, operands, mode);          \
  assert_eq(buf.data[index], expected);

  RUN_TEST(op_arr16, MODE_PROTECTED, 0, 0x66);

  op_write_prefix(&buf, op_addr16, MODE_PROTECTED);
  assert_eq(buf.data[1], 0x67);
  assert_eq(buf.data[2], 0x66);

  RUN_TEST(op_arr32, MODE_REAL, 3, 0x66);

  op_write_prefix(&buf, op_addr32, MODE_REAL);
  assert_eq(buf.data[4], 0x67);
  assert_eq(buf.data[5], 0x66);

  RUN_TEST(op_arr64, MODE_LONG, 6, REX_W);
  RUN_TEST(op_addr64, MODE_LONG, 7, REX_W);
  free(buf.data);
}

Test(operand, construct_operand) {
  const operand_t byte = op_construct_operand(OP_IMM8, 0, &(unsigned char){0xFF}, "");

  assert_eq(byte.type, OP_IMM8);
  assert_eq(byte.offset, 0);
  assert_eq(*(unsigned char *)byte.data, 0xFF);
}

Test(operand, ident_identify) {
  const enum operands input[] = {OP_R8, OP_R16, OP_NULL, OP_NULL};
  const enum operands input2[] = {OP_R8, OP_M16, OP_NULL, OP_NULL};

  assert_eq(op_ident_identify(input), ENC_MR);
  assert_eq(op_ident_identify(input2), ENC_RM);
}

Test(operand, modrm_mode) {
  struct {
    operand_t operand;
    int expected_mode;
  } test_cases[] = {
      {op_construct_operand(OP_M64, 0, &(enum registers){REG_RIP}, ""), OP_MODRM_INDIRECT},
      {op_construct_operand(OP_M64, 0, &(enum registers){REG_RAX}, ""), OP_MODRM_INDIRECT},
      {op_construct_operand(OP_M64, 8, &(enum registers){REG_RAX}, ""), OP_MODRM_DISP8},
      {op_construct_operand(OP_M64, 0, &(enum registers){REG_RBP}, ""), OP_MODRM_DISP8},
      {op_construct_operand(OP_M64, 0xFFFF, &(enum registers){REG_RAX}, ""), OP_MODRM_DISP32},
  };

  for (size_t i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
    assert_eq(op_modrm_mode(test_cases[i].operand), test_cases[i].expected_mode);
  }
}

Test(operand, write_opcode) {
  const operand_t op_arr[] = {r64, imm64, OP_NONE, OP_NONE};
  const instr_encode_table_t instr_ref = {ENC_MI, 0, {0xC7}, MODE_SUPPORT_ALL, {0xC6}, 1, NULL, true};

  uint8_t *out = op_write_opcode(op_arr, &instr_ref);
  assert_eq(out, instr_ref.opcode);

  const operand_t op_arr_byte[] = {r8, imm8, OP_NONE, OP_NONE};
  out = op_write_opcode(op_arr_byte, &instr_ref);
  assert_eq(out, instr_ref.byte_instr_opcode);
}

int main(void) {
  TestSuite(operand);

  RunTest(operand, write_prefix);
  RunTest(operand, construct_operand);
  RunTest(operand, ident_identify);
  RunTest(operand, modrm_mode);
  RunTest(operand, write_opcode);

  return 0;
}
