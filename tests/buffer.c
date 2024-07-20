#include "buffer.h"
#include <criterion/criterion.h>
#include <stdlib.h>

Test(buffer, write) {
  const uint8_t data[] = {0xBE, 0xEF};
  buffer_t buf = BUF_NULL;

  buf_write(&buf, data, sizeof(data));

  cr_assert(buf.data[0] == 0xBE);
  cr_assert(buf.data[1] == 0xEF);
  cr_assert(buf.len == sizeof(data));
}