#include "codegen.h"
#include "init.h"
#include "labrador/labrador.h"
#include "null.h"
#include <stdlib.h>

TEST() {
  jasInstance_t realModeInstance;
  jasInitNew(JAS_MODE_16, &realModeInstance, JAS_NULL, JAS_NULL);

  const jasErrorCode_t realModeErrorCode = jasCodegen(AAD, JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

  SHOULD_EQUAL(realModeErrorCode, JAS_NO_ERROR);
  SHOULD_EQUAL(realModeInstance.buffer[0], 0xD5);
  SHOULD_EQUAL(realModeInstance.buffer[1], 0x0A);

  const uint8_t argContent = 10;
  const jasTaggedOperand_t realModeOperand = jasConstructOperand((void *)&argContent, JAS_OPERAND_8);

  const jasErrorCode_t realModeOperandErrorCode = jasCodegen(AAD, realModeOperand, JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

  SHOULD_EQUAL(realModeOperandErrorCode, JAS_NO_ERROR);
  SHOULD_EQUAL(realModeInstance.buffer[2], 0xD5);
  SHOULD_EQUAL(realModeInstance.buffer[3], 0x0A);

  free(realModeInstance.buffer);

  jasInstance_t longModeInstance;
  jasInitNew(JAS_MODE_64, &longModeInstance, JAS_NULL, JAS_NULL);

  const jasErrorCode_t longModeErrorCode = jasCodegen(AAD, JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, &longModeInstance);

  SHOULD_EQUAL(longModeErrorCode, JAS_NON_LONG_MODE_INSTRUCTION);
  SHOULD_EQUAL(longModeInstance.bufferLen, 0);

  free(longModeInstance.buffer);

  PASS
}
