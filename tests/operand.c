#include "operand.h"
#include "buffer.h"
#include "rex.h"
#include <criterion/criterion.h>

Test(operand, write_prefix) {
  const operand_t byte = op_construct_operand(OP_IMM8, 0, &(unsigned char){0xFF});
  const operand_t word = op_construct_operand(OP_IMM16, 0, &(unsigned short){0xFFFF});
  const operand_t dword = op_construct_operand(OP_IMM32, 0, &(unsigned int){0xFFFFFFFF});
  const operand_t qword = op_construct_operand(OP_IMM64, 0, &(unsigned long){0xFFFFFFFFFFFFFFFF});

  const operand_t op_arr[] = {byte, word, dword, qword};

  buffer_t prefix = op_write_prefix(op_arr);

  cr_assert(buf_element_exists(&prefix, OP_WORD_OVERRIDE));
  cr_assert(buf_element_exists(&prefix, REX_W));
  cr_assert(!buf_element_exists(&prefix, OP_ADDR_OVERRIDE));

  free(prefix.data);
}

Test(operand, construct_operand) {
  const operand_t byte = op_construct_operand(OP_IMM8, 0, &(unsigned char){0xFF});

  cr_assert_eq(byte.type, OP_IMM8);
  cr_assert_eq(byte.offset, 0);
  cr_assert_eq(*(unsigned char *)byte.data, 0xFF);
}

Test(operand, size_of) {
  cr_assert_eq(op_sizeof(OP_IMM8), 8);
  cr_assert_eq(op_sizeof(OP_IMM16), 16);
  cr_assert_eq(op_sizeof(OP_IMM32), 32);
  cr_assert_eq(op_sizeof(OP_IMM64), 64);

  cr_assert_eq(op_sizeof(OP_R8), 8);
  cr_assert_eq(op_sizeof(OP_R16), 16);
  cr_assert_eq(op_sizeof(OP_R32), 32);
  cr_assert_eq(op_sizeof(OP_R64), 64);

  cr_assert_eq(op_sizeof(OP_M8), 8);
  cr_assert_eq(op_sizeof(OP_M16), 16);
  cr_assert_eq(op_sizeof(OP_M32), 32);
  cr_assert_eq(op_sizeof(OP_M64), 64);

  cr_assert_eq(op_sizeof(OP_NULL), 0);
}