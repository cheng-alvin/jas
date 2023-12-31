
#include "error.h"
#include "init.h"
#include "macro.h"
#include "modrm.h"
#include "operand.h"

jasErrorCode_t jasExtendedOperandIdentityRM(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance, uint8_t mode) {
  CONDITIONAL_WRITE((op2.type == JAS_REG_OPERAND_8 || op2.type == JAS_INDIRECT_8) && op1.type == JAS_REG_OPERAND_8, jasGenerateModrm(mode, op1.operand.reg.reg8, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg8 : op1.operand.reg.reg8))
  CONDITIONAL_WRITE((op2.type == JAS_REG_OPERAND_16 || op2.type == JAS_INDIRECT_16) && op1.type == JAS_REG_OPERAND_16, jasGenerateModrm(mode, op1.operand.reg.reg16, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg16 : op1.operand.reg.reg16))
  CONDITIONAL_WRITE((op2.type == JAS_REG_OPERAND_32 || op2.type == JAS_INDIRECT_32) && op1.type == JAS_REG_OPERAND_32, jasGenerateModrm(mode, op1.operand.reg.reg32, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg32 : op1.operand.reg.reg32))
  CONDITIONAL_WRITE((op2.type == JAS_REG_OPERAND_64 || op2.type == JAS_INDIRECT_64) && op1.type == JAS_REG_OPERAND_64, jasGenerateModrm(mode, op1.operand.reg.reg64, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg64 : op1.operand.reg.reg64))
  return JAS_NO_ERROR;
}