#include "aas.h"
#include "init.h"
#include "labrador.h"
#include "null.h"
#include <stdlib.h>

TEST(aas) {
  jasInstance_t *instance;
  uint8_t *buffer = malloc(1); // TODO Affected by adding allocation for `buffer`
  const jasErrorCode_t initExitStatus = jasInitNew(JAS_MODE_32, instance, buffer, NULL);
  SHOULD_EQUAL(initExitStatus, JAS_NO_ERROR)

  jasErrorCode_t aasExitStatus = aas(JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, instance);
  SHOULD_EQUAL(aasExitStatus, JAS_NO_ERROR)
  SHOULD_EQUAL(buffer[0], 0x3F)

  instance->mode = JAS_MODE_64;
  aasExitStatus = aas(JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, instance);
  SHOULD_EQUAL(aasExitStatus, JAS_NON_LONG_MODE_INSTRUCTION)

  PASS
}