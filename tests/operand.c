#include "operand.h"
#include "buffer.h"
#include "encoder.h"
#include "rex.h"
#include "test.h"

Test(operand, write_prefix) {
  /**
   * mode | size    | output
   * -----------------------------
   * real | 32      | 0x66
   * real | 32 addr | 0x67
   * prot | 16      | 0x66
   * prot | 16 addr | 0x67
   * long | 16      | 0x66
   * long | 32 addr | 0x67
   * long | 16 addr | 0x67 + 0x66
   *
   * long | 64      | REX.W
   *
   * Additional: Register REX prefix for REX.B
   *
   * @note (Potentially drop support for 16-bit operands
   * in long mode)
   *
   * @note Note that when a 16 bit operand override is
   * used in an instruction, all of the operands in the
   * instruction will be changed to that particular si-
   * ze and will be override.
   */

  buffer_t buf = BUF_NULL;
  const operand_t op_arr[] = {
      op_construct_operand(OP_R8, 0, &(enum registers){REG_RAX}),
      op_construct_operand(OP_IMM8, 0, &(unsigned char){0xFF}),
  };

  op_write_prefix(&buf, op_arr, MODE_REAL);
  op_write_prefix(&buf, op_arr, MODE_LONG);
  op_write_prefix(&buf, op_arr, MODE_PROTECTED);

  assert_eq(buf.data[2], OP_WORD_OVERRIDE);
  assert_eq(buf.data[1], REX_W);
  assert_eq(buf.data[0], OP_WORD_OVERRIDE);
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
