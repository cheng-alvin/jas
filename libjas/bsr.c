#include "codegen.h"
#include "error.h"
#include "init.h"
#include "macro.h"
#include "modrm.h"
#include "null.h"
#include "operand.h"
#include "remove.h"
#include "rex.h"
#include "rm.h"
#include "write.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

jasErrorCode_t jasBSR(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  jasModrmMode_t mode = JAS_NULL;

  signed long long indexOfRex = instance->bufferLen;
  CONDITIONAL_WRITE(jasRexExpectedInRegisterEncoding(op2), jasRexConstructPrefix(JAS_NULL, JAS_REX_R))
  else {
    WRITE(0)
  }

  // ??? (line 26 - 31) Why is long mode not allowed ???
  if (op1.type == JAS_REG_OPERAND_64) {
    if (instance->mode == JAS_MODE_64)
      return JAS_NON_LONG_MODE_INSTRUCTION;

    instance->buffer[indexOfRex] = jasRexConstructPrefix(instance->buffer[indexOfRex], JAS_REX_W);
  }

  switch (op2.type) {
  case JAS_INDIRECT_32:
  case JAS_INDIRECT_64:
  case JAS_INDIRECT_16:
    mode = JAS_MODRM_INDIRECT;
    break;

  case JAS_REG_OPERAND_16:
  case JAS_REG_OPERAND_32:
  case JAS_REG_OPERAND_64:
    mode = JAS_MODRM_REGISTER;
    break;

  default:
    return JAS_OPERAND_ERROR;
  }

  WRITE(0x0F)
  WRITE(0xBD)
  if (instance->buffer[indexOfRex] == 0) {
    instance->buffer = removeElement(instance->buffer, instance->bufferLen, indexOfRex);

    instance->bufferLen--;
  }

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, mode, JAS_NULL);
}