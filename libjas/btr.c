#include "codegen.h"
#include "error.h"
#include "init.h"
#include "macro.h"
#include "mi.h"
#include "mode.h"
#include "modrm.h"
#include "mr.h"
#include "null.h"
#include "operand.h"
#include "remove.h"
#include "rex.h"
#include "rm.h"
#include "write.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static jasErrorCode_t encodeOpcode(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance);

jasErrorCode_t jasBTR(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  signed long long indexOfRex = instance->bufferLen;
  CONDITIONAL_WRITE(jasRexExpectedInRegisterEncoding(op1), jasRexConstructPrefix(JAS_NULL, JAS_REX_R))
  else {
    WRITE(0)
  }

  const jasErrorCode_t opcodeStatus = encodeOpcode(op1, op2, op3, op4, instance);
  if (opcodeStatus != JAS_NO_ERROR)
    return opcodeStatus;

  if (instance->buffer[indexOfRex] == 0) {
    instance->buffer = removeElement(instance->buffer, instance->bufferLen, indexOfRex);

    instance->bufferLen--;
  }

  if (instance->buffer[instance->bufferLen - 1] == 0xB3)
    return jasExtendedOperandIdentityMR(op1, op2, op3, op4, instance, jasGetMode(op1, op2), JAS_NULL);

  return jasExtendedOperandIdentityMI(op1, op2, op3, op4, instance, jasGetMode(op1, op2), 6);
}

static jasErrorCode_t encodeOpcode(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  switch (op2.type) {
  case JAS_REG_OPERAND_64:
  case JAS_REG_OPERAND_32:
  case JAS_REG_OPERAND_16:
    WRITE(0x0F)
    WRITE(0xB3)
    break;

  case JAS_OPERAND_8:
    WRITE(0x0F)
    WRITE(0xBA)
    break;

  default:
    return JAS_OPERAND_ERROR;
  }
}