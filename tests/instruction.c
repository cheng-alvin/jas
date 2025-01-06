#include "instruction.h"
#include "test.h"

Test(instr, instr_gen) {
  instruction_t instr = instr_gen(INSTR_MOV, 2, OP_R64, REG_RAX, 0, OP_IMM64, 0x0, 0);
  assert_eq(instr.instr, INSTR_MOV);

  assert_eq(instr.operands[0].type, OP_R64);
  assert_eq(*(enum registers *)instr.operands[0].data, REG_RAX);
  assert_eq(instr.operands[0].offset, 0);

  assert_eq(instr.operands[1].type, OP_IMM64);
  assert_eq(*(uint64_t *)instr.operands[1].data, 0x0);
  assert_eq(instr.operands[1].offset, 0);

  assert_eq(instr.operands[2].type, OP_NULL);
  assert_eq(instr.operands[3].type, OP_NULL);

  instr = instr_gen(INSTR_MOV, 2, OP_R64, REG_RAX, 0, OP_REL32, "label", 0);

  assert_eq(instr.instr, INSTR_MOV);

  assert_eq(instr.operands[0].type, OP_R64);
  assert_eq(*(enum registers *)instr.operands[0].data, REG_RAX);
  assert_eq(instr.operands[0].offset, 0);

  assert_eq(instr.operands[1].type, OP_REL32);
  assert_str_eq((char *)instr.operands[1].label, "label", "Expected label to be 'label'\n");
  assert_eq(instr.operands[1].offset, 0);

  assert_eq(instr.operands[2].type, OP_NULL);
  assert_eq(instr.operands[3].type, OP_NULL);
}

int main(void) {
  TestSuite(instr);

  RunTest(instr, instr_gen);
  return 0;
}