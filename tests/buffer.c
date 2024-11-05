#include "buffer.h"
#include "test.h"
#include <stdbool.h>
#include <stdlib.h>

Test(buffer, write) {
  const uint8_t data[] = {0xBE, 0xEF};
  buffer_t buf = BUF_NULL;

  buf_write(&buf, data, sizeof(data));

  assert(buf.data[0] = 0xBE);
  assert(buf.data[1] == 0xEF);
  assert(buf.len == sizeof(data));

  free(buf.data);
}

Test(buffer, write_byte) {
  buffer_t buf = BUF_NULL;

  buf_write_byte(&buf, 0xBE);

  assert(buf.data[0] = 0xBE);
  assert(buf.len == 1);

  free(buf.data);
}

Test(buffer, remove) {
  buffer_t buf = BUF_NULL;

  buf_write_byte(&buf, 0xBE);
  buf_write_byte(&buf, 0xEF);

  buf_remove(&buf, 0);

  assert(buf.data[0] == 0xEF);
  assert(buf.len == 1);

  free(buf.data);
}

Test(buffer, element_exists) {
  buffer_t buf = BUF_NULL;

  buf_write_byte(&buf, 0xBE);

  assert(buf_element_exists(&buf, 0xBE));
  assert(!buf_element_exists(&buf, 0xEF));

  free(buf.data);
}

Test(buffer, concat) {
  buffer_t buf1 = BUF_NULL;
  buffer_t buf2 = BUF_NULL;

  buf_write_byte(&buf1, 0xBE);
  buf_write_byte(&buf2, 0xEF);

  buf_concat(&buf1, 1, &buf2);

  assert(buf1.data[0] == 0xBE);
  assert(buf1.data[1] == 0xEF);
  assert(buf1.len == 2);

  free(buf1.data);
  free(buf2.data);
}

int main(void) {
  TestSuite(buffer);

  RunTest(buffer, write);
  RunTest(buffer, write_byte);
  RunTest(buffer, remove);
  RunTest(buffer, element_exists);
  RunTest(buffer, concat);

  return 0;
}