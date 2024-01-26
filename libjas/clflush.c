#include "codegen.h"
#include "error.h"
#include "init.h"
#include "macro.h"
#include "modrm.h"
#include "null.h"
#include "rex.h"
#include "write.h"
#include <stdint.h>

jasErrorCode_t jasCLFLUSH(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  WRITE(0x0F)
  WRITE(0xAE)

  switch (op1.type) {
  case JAS_INDIRECT_8:
    WRITE(jasGenerateModrm(JAS_MODRM_INDIRECT, 7, op1.operand.reg.indirectReg8))
    break;

  case JAS_INDIRECT_16:
    WRITE(jasGenerateModrm(JAS_MODRM_INDIRECT, 7, op1.operand.reg.indirectReg16))
    break;

  case JAS_INDIRECT_32:
    WRITE(jasGenerateModrm(JAS_MODRM_INDIRECT, 7, op1.operand.reg.indirectReg32))
    break;

  case JAS_INDIRECT_64:
    WRITE(jasGenerateModrm(JAS_MODRM_INDIRECT, 7, op1.operand.reg.indirectReg64))
    break;
  }

  WRITE(0x10)
  WRITE(0x00)
  WRITE(0x00)
  WRITE(0x00)
}