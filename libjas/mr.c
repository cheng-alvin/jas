#include "error.h"
#include "init.h"
#include "macro.h"
#include "modrm.h"
#include "operand.h"
#include "write.h"

jasErrorCode_t jasExtendedOperandIdentityMR(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance, uint8_t mode, uint8_t opcodeExtention) {
  if (op1.type == JAS_REG_OPERAND_8 || op1.type == JAS_INDIRECT_8) {
    if (op1.type == JAS_REG_OPERAND_64_DISP) {
      jasGenerateModrm(JAS_MODRM_FOLLOWING_DISPLACEMENT_64, op2.operand.reg.reg8, jasGetRegField(op1.operand.reg.regDisp64.reg));

      WRITE16((op1.operand.reg.regDisp64.displacement >> 48) & 0xffff)
      WRITE16((op1.operand.reg.regDisp64.displacement >> 32) & 0xffff)
      WRITE16((op1.operand.reg.regDisp64.displacement >> 16) & 0xffff)
      WRITE16(op1.operand.reg.regDisp64.displacement & 0xffff)

      return;
    }

    if (op1.type == JAS_REG_OPERAND_8_DISP) {
      jasGenerateModrm(JAS_MODRM_FOLLOWING_DISPLACEMENT_8, op2.operand.reg.reg8, jasGetRegField(op1.operand.reg.regDisp8.reg));

      WRITE(op1.operand.reg.regDisp8.displacement)

      return;
    }

    jasGenerateModrm(mode, jasGetRegField(op2.operand.reg.reg8), mode == JAS_MODRM_INDIRECT ? jasGetRegField(op1.operand.reg.indirectReg8) : jasGetRegField(op1.operand.reg.reg8));
  }

  if (op1.type == JAS_REG_OPERAND_16 || op1.type == JAS_INDIRECT_16) {
    if (op1.type == JAS_REG_OPERAND_64_DISP) {
      jasGenerateModrm(JAS_MODRM_FOLLOWING_DISPLACEMENT_64, op2.operand.reg.reg16, jasGetRegField(op1.operand.reg.regDisp64.reg));

      WRITE16((op1.operand.reg.regDisp64.displacement >> 48) & 0xffff)
      WRITE16((op1.operand.reg.regDisp64.displacement >> 32) & 0xffff)
      WRITE16((op1.operand.reg.regDisp64.displacement >> 16) & 0xffff)
      WRITE16(op1.operand.reg.regDisp64.displacement & 0xffff)

      return;
    }

    if (op1.type == JAS_REG_OPERAND_8_DISP) {
      jasGenerateModrm(JAS_MODRM_FOLLOWING_DISPLACEMENT_8, op2.operand.reg.reg16, jasGetRegField(op1.operand.reg.regDisp8.reg));

      WRITE(op1.operand.reg.regDisp8.displacement)

      return;
    }

    jasGenerateModrm(mode, jasGetRegField(op2.operand.reg.reg16), mode == JAS_MODRM_INDIRECT ? jasGetRegField(op1.operand.reg.indirectReg16) : jasGetRegField(op1.operand.reg.reg16));
  }

  if (op1.type == JAS_REG_OPERAND_32 || op1.type == JAS_INDIRECT_32) {
    if (op1.type == JAS_REG_OPERAND_64_DISP) {
      jasGenerateModrm(JAS_MODRM_FOLLOWING_DISPLACEMENT_64, op2.operand.reg.reg32, jasGetRegField(op1.operand.reg.regDisp64.reg));

      WRITE16((op1.operand.reg.regDisp64.displacement >> 48) & 0xffff)
      WRITE16((op1.operand.reg.regDisp64.displacement >> 32) & 0xffff)
      WRITE16((op1.operand.reg.regDisp64.displacement >> 16) & 0xffff)
      WRITE16(op1.operand.reg.regDisp64.displacement & 0xffff)

      return;
    }

    if (op1.type == JAS_REG_OPERAND_8_DISP) {
      jasGenerateModrm(JAS_MODRM_FOLLOWING_DISPLACEMENT_8, op2.operand.reg.reg32, jasGetRegField(op1.operand.reg.regDisp8.reg));

      WRITE(op1.operand.reg.regDisp8.displacement)

      return;
    }

    jasGenerateModrm(mode, jasGetRegField(op2.operand.reg.reg32), mode == JAS_MODRM_INDIRECT ? jasGetRegField(op1.operand.reg.indirectReg32) : jasGetRegField(op1.operand.reg.reg32));
  }

  if (op1.type == JAS_REG_OPERAND_64 || op1.type == JAS_INDIRECT_64) {
    if (op1.type == JAS_REG_OPERAND_64_DISP) {
      jasGenerateModrm(JAS_MODRM_FOLLOWING_DISPLACEMENT_64, op2.operand.reg.reg64, jasGetRegField(op1.operand.reg.regDisp64.reg));

      WRITE16((op1.operand.reg.regDisp64.displacement >> 48) & 0xffff)
      WRITE16((op1.operand.reg.regDisp64.displacement >> 32) & 0xffff)
      WRITE16((op1.operand.reg.regDisp64.displacement >> 16) & 0xffff)
      WRITE16(op1.operand.reg.regDisp64.displacement & 0xffff)

      return;
    }

    if (op1.type == JAS_REG_OPERAND_8_DISP) {
      jasGenerateModrm(JAS_MODRM_FOLLOWING_DISPLACEMENT_8, op2.operand.reg.reg64, jasGetRegField(op1.operand.reg.regDisp8.reg));

      WRITE(op1.operand.reg.regDisp8.displacement)

      return;
    }

    jasGenerateModrm(mode, jasGetRegField(op2.operand.reg.reg64), mode == JAS_MODRM_INDIRECT ? jasGetRegField(op1.operand.reg.indirectReg64) : jasGetRegField(op1.operand.reg.reg64));
  }

  return JAS_NO_ERROR;
}
