#include "codegen.h"
#include "error.h"
#include "init.h"
#include "macro.h"
#include "null.h"
#include "write.h"
#include <stdint.h>

jasErrorCode_t aam(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  if (instance->mode == JAS_MODE_64)
    return JAS_NON_LONG_MODE_INSTRUCTION;

  WRITE(0xD4)

  if (op1.type == JAS_NO_OPERAND_TYPE) {
    WRITE(0x0A)
    return JAS_NO_ERROR;
  }

  WRITE(op1.operand.operand8)
  return JAS_NO_ERROR;
}
