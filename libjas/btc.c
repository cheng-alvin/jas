
#include "codegen.h"
#include "error.h"
#include "init.h"
#include "macro.h"
#include "mi.h"
#include "modrm.h"
#include "mr.h"
#include "null.h"
#include "operand.h"
#include "rex.h"
#include "rm.h"
#include "write.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static jasModrmMode_t getMode(jasTaggedOperand_t op1, jasTaggedOperand_t op2);

static jasErrorCode_t encodeOpcode(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance);

jasErrorCode_t jasBTC(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  signed long long indexOfRex = instance->bufferLen;
  CONDITIONAL_WRITE(jasRexExpectedInRegisterEncoding(op1), jasRexConstructPrefix(NULL, JAS_REX_R))
  else {
    WRITE(0)
  }

  const jasErrorCode_t opcodeStatus = encodeOpcode(op1, op2, op3, op4, instance);
  if (opcodeStatus != JAS_NO_ERROR)
    return opcodeStatus;

  if (instance->buffer[indexOfRex] == 0)
    instance->buffer = removeElement(instance->buffer, instance->bufferLen, indexOfRex);

  if (instance->buffer[instance->bufferLen - 1] == 0xBB)
    return jasExtendedOperandIdentityMR(op1, op2, op3, op4, instance, getMode(op1, op2), NULL);

  return jasExtendedOperandIdentityMI(op1, op2, op3, op4, instance, getMode(op1, op2), 7);
}

static jasErrorCode_t encodeOpcode(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  switch (op2.type) {
  case JAS_REG_OPERAND_64:
  case JAS_REG_OPERAND_32:
  case JAS_REG_OPERAND_16:
    WRITE(0x0F)
    WRITE(0xBB)
    break;

  case JAS_OPERAND_8:
    WRITE(0x0F)
    WRITE(0xBA)
    break;

  default:
    return JAS_OPERAND_ERROR;
  }
}

// TODO Isolate?

static jasModrmMode_t getMode(jasTaggedOperand_t op1, jasTaggedOperand_t op2) {
  switch (op1.type) {
  case JAS_REG_OPERAND_64:
  case JAS_REG_OPERAND_32:
  case JAS_REG_OPERAND_16:
    return JAS_MODRM_REGISTER;

  case JAS_INDIRECT_64:
  case JAS_INDIRECT_32:
  case JAS_INDIRECT_16:
    return JAS_MODRM_INDIRECT;
  }
}
