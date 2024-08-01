#include "buffer.h"
#include "helper.h"
#include <check.h>
#include <stdbool.h>
#include <stdlib.h>

START_TEST(_write) {
  const uint8_t data[] = {0xBE, 0xEF};
  buffer_t buf = BUF_NULL;

  buf_write(&buf, data, sizeof(data));

  ck_assert(buf.data[0] = 0xBE);
  ck_assert(buf.data[1] == 0xEF);
  ck_assert(buf.len == sizeof(data));

  free(buf.data);
}
END_TEST

START_TEST(_write_byte) {
  buffer_t buf = BUF_NULL;

  buf_write_byte(&buf, 0xBE);

  ck_assert(buf.data[0] = 0xBE);
  ck_assert(buf.len == 1);

  free(buf.data);
}
END_TEST

START_TEST(_remove) {
  buffer_t buf = BUF_NULL;

  buf_write_byte(&buf, 0xBE);
  buf_write_byte(&buf, 0xEF);

  buf_remove(&buf, 0);

  ck_assert(buf.data[0] == 0xEF);
  ck_assert(buf.len == 1);

  free(buf.data);
}
END_TEST

START_TEST(_element_exists) {
  buffer_t buf = BUF_NULL;

  buf_write_byte(&buf, 0xBE);

  ck_assert(buf_element_exists(&buf, 0xBE));
  ck_assert(!buf_element_exists(&buf, 0xEF));

  free(buf.data);
}
END_TEST

Suite *_endian(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("_buffer");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, _write);
  tcase_add_test(tc_core, _write_byte);
  tcase_add_test(tc_core, _remove);
  tcase_add_test(tc_core, _element_exists);

  suite_add_tcase(s, tc_core);

  return s;
}

RUN_TEST_SUITE(_endian)
