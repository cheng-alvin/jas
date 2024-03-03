#include "error.c"
#include "macro.h"
#include "mr.h"
#include "null.h"
#include "operand.c"
#include <stdint.h>

// TODO Added unit tests
// TODO add to enum and lists
//
jasErrorCode_t jasARPL(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  if (instance->mode == JAS_MODE_64)
    return JAS_NON_LONG_MODE_INSTRUCTION;

  WRITE(0x63)
  jasExtendedOperandIdentityMR(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER, JAS_NULL);

  return JAS_NO_ERROR;
}
