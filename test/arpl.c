#include "codegen.h"
#include "init.h"
#include "labrador/labrador.h"
#include "null.h"
#include <stdlib.h>

TEST() {
  jasInstance_t realModeInstance;
  jasInitNew(JAS_MODE_16, &realModeInstance, JAS_NULL, JAS_NULL);

  const jasReg16_t argContent = JAS_REG_AX;
  const jasTaggedOperand_t registerOperand = jasConstructOperand((void *)&argContent, JAS_OPERAND_16);

  const jasErrorCode_t realModeOperandErrorCode = jasCodegen(ARPL, registerOperand, registerOperand, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

  SHOULD_EQUAL(realModeOperandErrorCode, JAS_NO_ERROR);
  SHOULD_EQUAL(realModeInstance.buffer[2], 0x63);
  SHOULD_EQUAL(realModeInstance.buffer[3], 0xC0);

  free(realModeInstance.buffer);

  jasInstance_t longModeInstance;
  jasInitNew(JAS_MODE_64, &longModeInstance, JAS_NULL, JAS_NULL);

  const jasErrorCode_t longModeErrorCode = jasCodegen(ARPL, registerOperand, registerOperand, JAS_NO_OPERAND, JAS_NO_OPERAND, &longModeInstance);

  SHOULD_EQUAL(longModeErrorCode, JAS_NON_LONG_MODE_INSTRUCTION);
  SHOULD_EQUAL(longModeInstance.bufferLen, 0);

  free(longModeInstance.buffer);

  PASS
}
