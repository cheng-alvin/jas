#include "endian.h"
#include <criterion/criterion.h>

Test(endian, endian) {
  uint8_t data[] = {0xBE, 0xEF};
  const uint8_t expected[] = {0xEF, 0xBE};

  uint8_t *result = endian(data, sizeof(data));

  cr_assert(result[0] == expected[0]);
  cr_assert(result[1] == expected[1]);

  free(result);
}