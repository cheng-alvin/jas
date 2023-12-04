#include "codegen.h"
#include "error.h"
#include "init.h"
#include "macro.h"
#include "null.h"
#include "write.h"
#include <stdint.h>

jasErrorCode_t aas(jasOperand_t op1, jasOperand_t op2, jasOperand_t op3, jasOperand_t op4, jasInstance_t *instance) {
  if (instance->mode == JAS_MODE_64)
    return JAS_ERROR_NON_LONG_MODE_INSTRUCTION;

  WRITE(0x3F)

  return JAS_NO_ERROR;
}