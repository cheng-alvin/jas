#include "buffer.h"
#include "codegen.h"
#include "error.h"
#include "instruction.h"
#include "register.h"
#include "test.h"

#define INIT(x, op1, op2, op3, op4)                \
  (instruction_t) {                                \
    .instr = x,                                    \
    .operands = (operand_t[]){op1, op2, op3, op4}, \
  }

Test(mov, mr) {
  const instruction_t instr[] =
      {
          INIT(INSTR_MOV, m64, r64, OP_NONE, OP_NONE),
          INIT(INSTR_MOV, m32, r32, OP_NONE, OP_NONE),
          INIT(INSTR_MOV, m16, r16, OP_NONE, OP_NONE),
          INIT(INSTR_MOV, m8, r8, OP_NONE, OP_NONE),
      };

  err_add_callback(&test_error_handler);
  buffer_t buf = codegen(MODE_LONG, instr, sizeof(instr));

  assert_not_null(buf.data);

  assert(buf.data[0] == 0x48);
  assert(buf.data[1] == 0x89);
  assert(buf.data[2] == 0x80);
  assert(buf.data[3] == 0xff);
  assert(buf.data[4] == 0x00);
  assert(buf.data[5] == 0x00);
  assert(buf.data[6] == 0x00);

  free(buf.data);
}

int main(void) {
  TestSuite(mov);
  RunTest(mov, mr);
}
