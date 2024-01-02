#include "codegen.h"
#include "error.h"
#include "init.h"
#include "macro.h"
#include "null.h"
#include "rex.h"
#include "write.h"
#include <stdint.h>

jasErrorCode_t jasCDQE(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  NO_LONG_MODE
  WRITE(jasRexConstructPrefix(NULL, JAS_REX_W))
  WRITE(0x98)
  return JAS_NO_ERROR;
}