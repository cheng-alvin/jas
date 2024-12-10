#include "endian.h"
#include "test.h"

Test(endian, endian) {
  uint8_t data[] = {0xBE, 0xEF};
  const uint8_t expected[] = {0xEF, 0xBE};

  uint8_t *result = endian(data, sizeof(data));

  assert(result[0] == expected[0]);
  assert(result[1] == expected[1]);
}

int main(void) {
  TestSuite(endian);
  RunTest(endian, endian);

  return 0;
}
