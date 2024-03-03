#include "codegen.h"
#include "init.h"
#include "instruction.h"
#include "labrador/labrador.h"
#include "null.h"
#include "operand.h"
#include <stdlib.h>

TEST() {
  jasInstance_t realModeInstance;
  jasInitNew(JAS_MODE_16, &realModeInstance, JAS_NULL, JAS_NULL);

  const jasErrorCode_t realModeErrorCode = jasCodegen(AAM, JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

  SHOULD_EQUAL(realModeErrorCode, JAS_NO_ERROR);
  SHOULD_EQUAL(realModeInstance.buffer[0], 0xD4);
  SHOULD_EQUAL(realModeInstance.buffer[1], 0x0A);

  const uint8_t argContent = 1;
  const jasTaggedOperand_t realModeOperand = jasConstructOperand((void *)&argContent, JAS_OPERAND_8);

  const jasErrorCode_t realModeOperandErrorCode = jasCodegen(AAM, realModeOperand, JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

  SHOULD_EQUAL(realModeOperandErrorCode, JAS_NO_ERROR);
  SHOULD_EQUAL(realModeInstance.buffer[2], 0xD4);
  SHOULD_EQUAL(realModeInstance.buffer[3], 0x01);

  free(realModeInstance.buffer);

  jasInstance_t longModeInstance;
  jasInitNew(JAS_MODE_64, &longModeInstance, JAS_NULL, JAS_NULL);

  const jasErrorCode_t longModeErrorCode = jasCodegen(AAM, JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, JAS_NO_OPERAND, &longModeInstance);

  SHOULD_EQUAL(longModeErrorCode, JAS_NON_LONG_MODE_INSTRUCTION);
  SHOULD_EQUAL(longModeInstance.bufferLen, 0);

  free(longModeInstance.buffer);

  PASS
}
