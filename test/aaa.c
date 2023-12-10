#include "aaa.h"
#include "init.h"
#include "labrador.h"
#include "null.h"
#include <stdlib.h>

TEST(aaa) {
  jasInstance_t instance;
  uint8_t *buffer = malloc(1); // TODO Affected by adding allocation for `buffer`
  const jasErrorCode_t initExitStatus = jasInitNew(JAS_MODE_32, &instance, buffer, NULL, NULL, NULL, false);
  SHOULD_EQUAL(initExitStatus, JAS_NO_ERROR)

  jasErrorCode_t aaaExitStatus = aaa(JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, &instance);
  SHOULD_EQUAL(aaaExitStatus, JAS_NO_ERROR)
  SHOULD_EQUAL(buffer[0], 0x37)

  instance.mode = JAS_MODE_64;
  aaaExitStatus = aaa(JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, &instance);
  SHOULD_EQUAL(aaaExitStatus, JAS_NON_LONG_MODE_INSTRUCTION)

  PASS
}