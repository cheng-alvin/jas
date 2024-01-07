#include "error.h"
#include "init.h"
#include "macro.h"
#include "operand.h"
#include "write.h"

jasErrorCode_t jasOperandIdentityI(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  ENCODE_IMMEDIATE(op2)
  return JAS_NO_ERROR;
}