#include "buffer.h"
#include <criterion/criterion.h>
#include <stdbool.h>
#include <stdlib.h>

Test(buffer, write) {
  const uint8_t data[] = {0xBE, 0xEF};
  buffer_t buf = BUF_NULL;

  buf_write(&buf, data, sizeof(data));

  cr_assert(buf.data[0] = 0xBE);
  cr_assert(buf.data[1] == 0xEF);
  cr_assert(buf.len == sizeof(data));

  free(buf.data);
}

Test(buffer, write_byte) {
  buffer_t buf = BUF_NULL;

  buf_write_byte(&buf, 0xBE);

  cr_assert(buf.data[0] = 0xBE);
  cr_assert(buf.len == 1);

  free(buf.data);
}

Test(buffer, remove) {
  buffer_t buf = BUF_NULL;

  buf_write_byte(&buf, 0xBE);
  buf_write_byte(&buf, 0xEF);

  buf_remove(&buf, 0);

  cr_assert(buf.data[0] == 0xEF);
  cr_assert(buf.len == 1);

  free(buf.data);
}

Test(buffer, element_exists) {
  buffer_t buf = BUF_NULL;

  buf_write_byte(&buf, 0xBE);

  cr_assert(buf_element_exists(&buf, 0xBE));
  cr_assert(!buf_element_exists(&buf, 0xEF));

  free(buf.data);
}
