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

  buffer_t buf = codegen(MODE_LONG, instr, sizeof(instr) / sizeof(instruction_t));

  err_add_callback(&test_error_handler);

  free(buf.data);
}

int main(void) {
  TestSuite(codegen);
  RunTest(codegen, mov);
}
