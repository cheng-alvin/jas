#include "codegen.h"
#include "error.h"
#include "init.h"
#include "macro.h"
#include "null.h"
#include "write.h"
#include <stdint.h>

jasErrorCode_t jasAAS(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  if (instance->mode == JAS_MODE_64)
    return JAS_NON_LONG_MODE_INSTRUCTION;

  WRITE(0x3F)

  return JAS_NO_ERROR;
}
