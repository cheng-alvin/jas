#include "operand.h"
#include "buffer.h"
#include "helper.h"
#include "rex.h"
#include <check.h>
#include <stdlib.h>

START_TEST(_write_prefix) {
  const operand_t byte = op_construct_operand(OP_IMM8, 0, &(unsigned char){0xFF});
  const operand_t word = op_construct_operand(OP_IMM16, 0, &(unsigned short){0xFFFF});
  const operand_t dword = op_construct_operand(OP_IMM32, 0, &(unsigned int){0xFFFFFFFF});
  const operand_t qword = op_construct_operand(OP_IMM64, 0, &(unsigned long){0xFFFFFFFFFFFFFFFF});

  const operand_t op_arr[] = {byte, word, dword, qword};

  buffer_t prefix = op_write_prefix(op_arr);

  ck_assert(buf_element_exists(&prefix, OP_WORD_OVERRIDE));
  ck_assert(buf_element_exists(&prefix, REX_W));
  ck_assert(!buf_element_exists(&prefix, OP_ADDR_OVERRIDE));

  free(prefix.data);
}
END_TEST

START_TEST(_construct_operand) {
  const operand_t byte = op_construct_operand(OP_IMM8, 0, &(unsigned char){0xFF});

  ck_assert(byte.type == OP_IMM8);
  ck_assert(byte.offset == 0);
  ck_assert(*(unsigned char *)byte.data == 0xFF);
}
END_TEST

START_TEST(_sizeof) {
  ck_assert(op_sizeof(OP_IMM8) == 8);
  ck_assert(op_sizeof(OP_IMM16) == 16);
  ck_assert(op_sizeof(OP_IMM32) == 32);
  ck_assert(op_sizeof(OP_IMM64) == 64);

  ck_assert(op_sizeof(OP_R8) == 8);
  ck_assert(op_sizeof(OP_R16) == 16);
  ck_assert(op_sizeof(OP_R32) == 32);
  ck_assert(op_sizeof(OP_R64) == 64);

  ck_assert(op_sizeof(OP_M8) == 8);
  ck_assert(op_sizeof(OP_M16) == 16);
  ck_assert(op_sizeof(OP_M32) == 32);
  ck_assert(op_sizeof(OP_M64) == 64);

  ck_assert(op_sizeof(OP_NULL) == 0);
}
END_TEST

START_TEST(_ident_identify) {
  const enum operands input[] = {OP_R8, OP_R16, OP_NULL, OP_NULL};
  const enum operands input2[] = {OP_R8, OP_M16, OP_NULL, OP_NULL};

  ck_assert(op_ident_identify(input) == OP_MR);
  ck_assert(op_ident_identify(input2) == OP_RM);
}
END_TEST

Suite *_operand(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("_operand");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, _write_prefix);
  tcase_add_test(tc_core, _construct_operand);
  tcase_add_test(tc_core, _sizeof);
  tcase_add_test(tc_core, _ident_identify);

  suite_add_tcase(s, tc_core);

  return s;
}

RUN_TEST_SUITE(_operand)