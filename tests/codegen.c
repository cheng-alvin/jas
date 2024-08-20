#include "codegen.h"
#include "buffer.h"
#include "register.h"
#include "test.h"

#define INIT(x, op1, op2, op3, op4)                  \
  {(instruction_t){                                  \
      .instr = x,                                    \
      .operands = (operand_t[]){op1, op2, op3, op4}, \
  }};

Test(codegen, mov) {
  const instruction_t instr[] =
      INIT(INSTR_MOV, m8, r8, OP_NONE, OP_NONE);

  err_add_callback(&test_error_handler);
  buffer_t buf = codegen(MODE_LONG, instr, sizeof(instr));

  assert_not_null(buf.data);

  assert(buf.data[0] == 0x88);
  assert(buf.data[1] == 0x40);
  assert(buf.data[2] == 0xFF);

  free(buf.data);
}

int main(void) {
  TestSuite(codegen);
  RunTest(codegen, mov);
}
