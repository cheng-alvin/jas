#include "codegen.h"
#include "error.h"
#include "init.h"
#include "macro.h"
#include "modrm.h"
#include "null.h"
#include "operand.h"
#include "rex.h"
#include "rm.h"
#include "write.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

jasErrorCode_t jasBSWAP(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  signed long long indexOfRex = instance->bufferLen;
  CONDITIONAL_WRITE(jasRexExpectedInRegisterEncoding(op1), jasRexConstructPrefix(NULL, JAS_REX_R))
  else {
    WRITE(0)
  }

  if (op1.type == JAS_REG_OPERAND_64) {
    NO_LONG_MODE
    instance->buffer[indexOfRex] = jasRexConstructPrefix(instance->buffer[indexOfRex], JAS_REX_W);
  }

  WRITE(0x0F)
  CONDITIONAL_WRITE(op1.type == JAS_REG_OPERAND_64, 0xC8 + (short)op1.operand.reg.reg64)
  else {
    WRITE(0xC0 + (short)op1.operand.reg.reg32)
  }

  return JAS_NO_ERROR;
}