#include "operand.h"
#include "buffer.h"
#include "encoder.h"
#include "rex.h"
#include "test.h"

Test(operand, write_prefix) {
  const operand_t op_arr[] = {imm8, imm16, imm32, imm64};

  // Sample function call
  // `buffer_t prefix = op_write_prefix(op_arr, mode);`

  /**
   * TODO test following conditions:
   *
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

  // Allow test to pass
}

Test(operand, construct_operand) {
  const operand_t byte = op_construct_operand(OP_IMM8, 0, &(unsigned char){0xFF});

  assert_eq(byte.type, OP_IMM8);
  assert_eq(byte.offset, 0);
  assert_eq(*(unsigned char *)byte.data, 0xFF);
}

Test(operand, sizeof) {
  assert_eq(op_sizeof(OP_IMM8), 8);
  assert_eq(op_sizeof(OP_IMM16), 16);
  assert_eq(op_sizeof(OP_IMM32), 32);
  assert_eq(op_sizeof(OP_IMM64), 64);

  assert_eq(op_sizeof(OP_R8), 8);
  assert_eq(op_sizeof(OP_R16), 16);
  assert_eq(op_sizeof(OP_R32), 32);
  assert_eq(op_sizeof(OP_R64), 64);

  assert_eq(op_sizeof(OP_M8), 8);
  assert_eq(op_sizeof(OP_M16), 16);
  assert_eq(op_sizeof(OP_M32), 32);
  assert_eq(op_sizeof(OP_M64), 64);

  assert_eq(op_sizeof(OP_NULL), 0);
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
  RunTest(operand, sizeof);
  RunTest(operand, ident_identify);

  return 0;
}
