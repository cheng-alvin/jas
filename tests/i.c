#include "operand.h"
#include "register.h"
#include "test.h"
#include <stdint.h>
#include "i.h"

Test(i, i) {
  const operand_t al = op_construct_operand(OP_R32, 0, &(enum registers){REG_EAX});
  const operand_t imm = op_construct_operand(OP_IMM32, 0, &(uint32_t){0xbeefface});
	
}

int main(void) {
  TestSuite(i);
  RunTest(i, i);

  return 0;
}
