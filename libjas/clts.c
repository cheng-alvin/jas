#include "codegen.h"
#include "error.h"
#include "init.h"
#include "macro.h"
#include "write.h"
#include <stdint.h>

jasErrorCode_t jasCLTS(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  WRITE(0x0F)
  WRITE(0x06)
  return JAS_NO_ERROR;
}
