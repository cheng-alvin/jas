#include "error.h"
#include "helper.h"
#include <check.h>
#include <stdlib.h>
#include <string.h>

void test_callback(const char *msg) {
  ck_assert_msg(
      !strcmp(msg, "msg"),
      "Error callback function was not called correctly.");
}

START_TEST(_add_callback) {
  err_add_callback(test_callback);

  ck_assert(err_callback);
  ck_assert(err_callback == test_callback);
}
END_TEST

START_TEST(_err) {
  err_add_callback(test_callback);
  err("msg");
}
END_TEST

Suite *_error(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("_error");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, _add_callback);
  tcase_add_test(tc_core, _err);

  suite_add_tcase(s, tc_core);

  return s;
}

RUN_TEST_SUITE(_error)