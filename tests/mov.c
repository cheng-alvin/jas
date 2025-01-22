#include "jas.h"
#include "test.h"

#define mr_bytes {0x48, 0x89, 0x80, 0xff, 0x00, 0x00, 0x00};
#define rm_bytes {0x48, 0x8B, 0x80, 0xff, 0x00, 0x00, 0x00};
#define oi_bytes {0x48, 0xB8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

#define mi_bytes \
  {0x48, 0xC7, 0x80, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF}

// TODO: Encapsulate this into a global macro for all tests
#define MOV_TEST(name, op1, op2, expected_bytes)                                        \
  Test(mov, name) {                                                                     \
    err_add_callback(test_error_handler);                                               \
                                                                                        \
    const operand_t operands[] = {op1, op2, OP_NONE, OP_NONE};                          \
    const instruction_t instr = {                                                       \
        .instr = INSTR_MOV,                                                             \
        .operands = &(operand_t[]){operands[0], operands[1], operands[2], operands[3]}, \
    };                                                                                  \
                                                                                        \
    const unsigned char expected[] = expected_bytes;                                    \
    const buffer_t buf = assemble_instr(MODE_LONG, &instr);                             \
                                                                                        \
    assert_eq_buf_arr(buf, expected, sizeof(expected));                                 \
    free(buf.data);                                                                     \
  }

MOV_TEST(mr, m64, r64, mr_bytes);
MOV_TEST(rm, r64, m64, rm_bytes);
MOV_TEST(oi, r64, imm64, oi_bytes);
MOV_TEST(mi, m64, imm64, oi_bytes);

int main(void) {
  TestSuite(mov);
  RunTest(mov, mr);
  RunTest(mov, rm);
  RunTest(mov, oi);
  RunTest(mov, mi);

  return 0;
}
