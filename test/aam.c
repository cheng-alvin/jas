#include "aam.h"
#include "init.h"
#include "labrador.h"
#include "null.h"
#include <stdlib.h>

TEST(aam) {
  jasInstance_t instance;
  uint8_t *buffer = malloc(1); // TODO Affected by adding allocation for `buffer`
  const jasErrorCode_t initExitStatus = jasInitNew(JAS_MODE_32, &instance, buffer, NULL, NULL, NULL, false);
  SHOULD_EQUAL(initExitStatus, JAS_NO_ERROR)

  jasErrorCode_t aamExitStatus = aam(JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, &instance);
  SHOULD_EQUAL(aamExitStatus, JAS_NO_ERROR)
  SHOULD_EQUAL(buffer[0], 0xD4)
  SHOULD_EQUAL(buffer[1], 0x0A)

  aamExitStatus = aam(&(jasOperand_t){.operand8 = 0xFF}, JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, &instance);
  SHOULD_EQUAL(aamExitStatus, JAS_NO_ERROR)
  SHOULD_EQUAL(buffer[2], 0xD4)
  SHOULD_EQUAL(buffer[3], 0xFF)

  instance.mode = JAS_MODE_64;
  aamExitStatus = aam(JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, &instance);
  SHOULD_EQUAL(aamExitStatus, JAS_NON_LONG_MODE_INSTRUCTION)

  PASS
}