#include "jas.h"
#include "test.h"

Test(mov, mr) {
  err_add_callback(test_error_handler);

  const instruction_t instr = {
      .instr = INSTR_MOV,
      .operands = (operand_t[]){m64, r64, OP_NONE, OP_NONE},
  };

  const buffer_t buf = assemble_instr(MODE_LONG, instr);

  unsigned char expected[] = {0x48, 0x89, 0x80, 0xff, 0x00, 0x00, 0x00};
  assert_eq_buf_arr(buf, expected, sizeof(expected));
  free(buf.data);
}

int main(void) {
  TestSuite(mov);
  RunTest(mov, mr);

  return 0;
}
