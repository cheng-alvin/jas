#include "endian.h"
#include "helper.h"
#include <check.h>
#include <stdlib.h>

START_TEST(__endian) {
  uint8_t data[] = {0xBE, 0xEF};
  const uint8_t expected[] = {0xEF, 0xBE};

  uint8_t *result = endian(data, sizeof(data));

  ck_assert(result[0] == expected[0]);
  ck_assert(result[1] == expected[1]);

  free(result);
}
END_TEST

Suite *_endian(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("_endian");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, __endian);

  suite_add_tcase(s, tc_core);

  return s;
}

RUN_TEST_SUITE(_endian)