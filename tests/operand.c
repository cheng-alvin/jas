#include "operand.h"
#include "buffer.h"
#include "encoder.h"
#include "rex.h"
#include "test.h"

Test(operand, write_prefix) {
  buffer_t buf = BUF_NULL;
  buf_write_byte(&buf, 0x0); // Filler byte cuz NULL pointer will be accessed if no prefix

  const operand_t op_arr16[] = {r16, imm16, OP_NONE, OP_NONE};
  const operand_t op_addr16[] = {m16, imm16, OP_NONE, OP_NONE};
  const operand_t op_arr32[] = {r32, imm32, OP_NONE, OP_NONE};
  const operand_t op_addr32[] = {m32, imm32, OP_NONE, OP_NONE};
  const operand_t op_arr64[] = {r64, imm64, OP_NONE, OP_NONE};
  const operand_t op_addr64[] = {m64, imm64, OP_NONE, OP_NONE};

#define RUN_TEST(operands, mode, index, expected) \
  op_write_prefix(&buf, operands, mode);          \
  assert_eq(buf.data[index], expected);

  RUN_TEST(op_arr32, MODE_REAL, 1, OP_WORD_OVERRIDE);
  RUN_TEST(op_addr32, MODE_REAL, 2, OP_ADDR_OVERRIDE);
  RUN_TEST(op_arr16, MODE_PROTECTED, 3, OP_WORD_OVERRIDE);
  RUN_TEST(op_arr16, MODE_LONG, 4, OP_WORD_OVERRIDE);

  RUN_TEST(op_addr16, MODE_LONG, 5, OP_ADDR_OVERRIDE);
  RUN_TEST(op_addr16, MODE_LONG, 6, OP_ADDR_OVERRIDE);

  RUN_TEST(op_arr16, MODE_LONG, 7, OP_WORD_OVERRIDE);
  RUN_TEST(op_addr32, MODE_LONG, 8, OP_ADDR_OVERRIDE);

  op_write_prefix(&buf, op_addr16, MODE_LONG);
  assert_eq(buf.data[9], OP_ADDR_OVERRIDE);
  assert_eq(buf.data[10], OP_WORD_OVERRIDE);

  RUN_TEST(op_arr64, MODE_LONG, 11, REX_W);

  free(buf.data);
}

Test(operand, construct_operand) {
  const operand_t byte = op_construct_operand(OP_IMM8, 0, &(unsigned char){0xFF});

  assert_eq(byte.type, OP_IMM8);
  assert_eq(byte.offset, 0);
  assert_eq(*(unsigned char *)byte.data, 0xFF);
}

Test(operand, ident_identify) {
  const enum operands input[] = {OP_R8, OP_R16, OP_NULL, OP_NULL};
  const enum operands input2[] = {OP_R8, OP_M16, OP_NULL, OP_NULL};

  assert_eq(op_ident_identify(input), OP_MR);
  assert_eq(op_ident_identify(input2), OP_RM);
}

int main(void) {
  TestSuite(operand);

  RunTest(operand, write_prefix);
  RunTest(operand, construct_operand);
  RunTest(operand, ident_identify);

  return 0;
}
