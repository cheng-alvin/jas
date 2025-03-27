#include "parse.h"
#include "test.h"

Test(parse, instr_name) {
  const char name[] = "CMOVAE"; // Sample instruction
  enum instructions instr = parse_instr_name(name);
  assert_eq(instr, INSTR_CMOVAE);
}

Test(parse, str_num) {
  const char hex[] = "0x1A";
  const char dec[] = "26";
  uint64_t hex_num = parse_str_num(hex);
  uint64_t dec_num = parse_str_num(dec);
  assert_eq(hex_num, 26);
  assert_eq(dec_num, 26);
}

Test(parse, reg) {
  const char name[] = "RAX";
  enum registers reg = parse_reg(name);
  assert_eq(reg, REG_RAX);
}

int main(void) {
  TestSuite(parse);
  RunTest(parse, instr_name);
  RunTest(parse, str_num);
  RunTest(parse, reg);

  return 0;
}
