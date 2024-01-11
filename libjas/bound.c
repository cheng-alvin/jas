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

// TODO Review required

// I couldn't find much documentation on how this instruction can be called.
// Therefore, I will be assuming the second `m16&16` operand is the same as
// `r/m16`. Until more sources can be found, there's not much I can do.

//! Note: Usage of m16&16 is not supported.

jasErrorCode_t jasBOUND(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  if (instance->mode == JAS_MODE_64)
    return JAS_NON_LONG_MODE_INSTRUCTION;

  WRITE(0x62)

  if (op1.type == JAS_REG_OPERAND_16)
    jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, op2.type == JAS_REG_OPERAND_16_DISP ? JAS_MODRM_FOLLOWING_DISPLACEMENT : JAS_MODRM_INDIRECT, JAS_NULL);
  else
    jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, op2.type == JAS_REG_OPERAND_32_DISP ? JAS_MODRM_FOLLOWING_DISPLACEMENT : JAS_MODRM_INDIRECT, JAS_NULL);
}
