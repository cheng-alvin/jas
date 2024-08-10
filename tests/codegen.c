#include "codegen.h"
#include "buffer.h"
#include "register.h"
#include "test.h"

Test(codegen, mov) {
  const instruction_t instr[] = {(instruction_t){
      .instr = INSTR_MOV,
      .operands = (operand_t[]){
          m8,
          r8,
          OP_NONE,
          OP_NONE,
      },
  }};

  err_add_callback(&test_error_handler);
  buffer_t buf = codegen(MODE_LONG, instr, sizeof(instr) / sizeof(instruction_t));

  assert_not_null(buf.data);

  assert(buf.data[0] == 0x88);
  assert(buf.data[1] == 0x40);
  assert(buf.data[2] == 0xFF);
  printf("buf.data[0]: %x\n", buf.data[0]);
  printf("buf.data[1]: %x\n", buf.data[1]);
  printf("buf.data[2]: %x\n", buf.data[2]);

  free(buf.data);
}

int main(void) {
  TestSuite(codegen);
  RunTest(codegen, mov);
}
