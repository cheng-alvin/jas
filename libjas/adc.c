#include "codegen.h"
#include "error.h"
#include "init.h"
#include "macro.h"
#include "modrm.h"
#include "null.h"
#include "operand.h"
#include "rex.h"
#include "write.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// TODO Maybe rename WRITE_IF_BREAKS() macro (this name sounds absolutely awful))
// TODO create new unit tests for `adc`

jasErrorCode_t adc(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  if (jasCheckIfHighRegistersAreValidUnderRexPrefix(op1, op2))
    return JAS_INVALID_HIGH_REGISTER;

  jasModrmMode_t mode;

  // TODO encapsulate behaviour
  // Note: If there's no REX prefix, therefore the `indexOfRex` will be 0 >
  signed long long indexOfRex = jasRexExpectedInRegisterEncoding(op1) ? instance->bufferLen : -1;
  WRITE_IF(jasRexExpectedInRegisterEncoding(op1), jasRexConstructPrefix(NULL, JAS_REX_B))

  switch (op1.type) {
  case JAS_INDIRECT_8:
    mode = JAS_MODRM_INDIRECT;
  case JAS_REG_OPERAND_8:
    mode = JAS_MODRM_REGISTER;

    WRITE_IF_BREAKS(op1.operand.reg.reg8 == JAS_REG_AL, 0x14)

    WRITE_IF_BREAKS(op1.type == JAS_REG_OPERAND_8 && (op2.type == JAS_REG_OPERAND_8 || op2.type == JAS_INDIRECT_8), 0x12)
    WRITE_IF_BREAKS(op2.type == JAS_REG_OPERAND_8, 0x10)

    WRITE(0x80)
    break;

  case JAS_INDIRECT_16:
    mode = JAS_MODRM_INDIRECT;
  case JAS_REG_OPERAND_16:
    mode = JAS_MODRM_REGISTER;

    // TODO encapsulate operand size override
    WRITE(0x66)
    WRITE_IF_BREAKS(op1.type == JAS_REG_OPERAND_16 && (op2.type == JAS_REG_OPERAND_16 || op2.type == JAS_INDIRECT_16), 0x13)
    WRITE_IF_BREAKS(op1.operand.reg.reg16 == JAS_REG_AX, 0x15)
    WRITE_IF_BREAKS(op2.type == JAS_OPERAND_8, 0x83)
    WRITE_IF_BREAKS(op2.type == JAS_REG_OPERAND_16, 0x11)

    WRITE(0x81)
    break;

  case JAS_INDIRECT_32:
    mode = JAS_MODRM_INDIRECT;
  case JAS_REG_OPERAND_32:
    mode = JAS_MODRM_REGISTER;

    WRITE_IF_BREAKS(op1.type == JAS_REG_OPERAND_32 && (op2.type == JAS_REG_OPERAND_32 || op2.type == JAS_INDIRECT_32), 0x13)
    WRITE_IF_BREAKS(op1.operand.reg.reg32 == JAS_REG_EAX, 0x15)
    WRITE_IF_BREAKS(op2.type == JAS_OPERAND_8, 0x83)
    WRITE_IF_BREAKS(op2.type == JAS_REG_OPERAND_32, 0x11)

    WRITE(0x81)
    break;

  case JAS_INDIRECT_64:
    mode = JAS_MODRM_INDIRECT;
  case JAS_REG_OPERAND_64:
    mode = JAS_MODRM_REGISTER;

    NO_LONG_MODE

    // TODO Encapsulate behaviour
    WRITE_IF(indexOfRex < 0, jasRexConstructPrefix(NULL, JAS_REX_W))
    else WRITE(jasRexConstructPrefix(instance->buffer[indexOfRex], JAS_REX_W));

    WRITE_IF_BREAKS(op1.type == JAS_REG_OPERAND_64 && (op2.type == JAS_REG_OPERAND_64 || op2.type == JAS_INDIRECT_64), 0x13)
    WRITE_IF_BREAKS(op1.operand.reg.reg64 == JAS_REG_RAX, 0x15)
    WRITE_IF_BREAKS(op2.type == JAS_OPERAND_8, 0x83)
    WRITE_IF_BREAKS(op2.type == JAS_REG_OPERAND_64, 0x11)

    WRITE(0x81)
    break;
  }

  // TODO encapsulate behaviour - Which is stupid how I did not encapsulate this behaviour in the first place.
  // Operand encoder:
  switch (instance->buffer[instance->bufferLen - 1]) {
  case 0x14:
  case 0x15:
    ENCODE_IMMEDIATE(op2)
    return JAS_OPERAND_ERROR;

  case 0x80:
  case 0x81:
  case 0x83:
    WRITE_IF(op1.type == JAS_REG_OPERAND_8 || op1.type == JAS_INDIRECT_8, jasGenerateModrm(mode, 2, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg8 : op1.operand.reg.reg8))
    WRITE_IF(op1.type == JAS_REG_OPERAND_16 || op1.type == JAS_INDIRECT_16, jasGenerateModrm(mode, 2, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg16 : op1.operand.reg.reg16))
    WRITE_IF(op1.type == JAS_REG_OPERAND_32 || op1.type == JAS_INDIRECT_32, jasGenerateModrm(mode, 2, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg32 : op1.operand.reg.reg32))
    WRITE_IF(op1.type == JAS_REG_OPERAND_64 || op1.type == JAS_INDIRECT_64, jasGenerateModrm(mode, 2, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg64 : op1.operand.reg.reg64))
    ENCODE_IMMEDIATE(op2)
    return JAS_OPERAND_ERROR;

  case 0x10:
  case 0x11:
    WRITE_IF_BREAKS(op1.type == JAS_REG_OPERAND_8 || op1.type == JAS_INDIRECT_8, jasGenerateModrm(mode, op2.operand.reg.reg8, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg8 : op1.operand.reg.reg8))
    WRITE_IF_BREAKS(op1.type == JAS_REG_OPERAND_16 || op1.type == JAS_INDIRECT_16, jasGenerateModrm(mode, op2.operand.reg.reg16, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg16 : op1.operand.reg.reg16))
    WRITE_IF_BREAKS(op1.type == JAS_REG_OPERAND_32 || op1.type == JAS_INDIRECT_32, jasGenerateModrm(mode, op2.operand.reg.reg32, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg32 : op1.operand.reg.reg32))
    WRITE_IF_BREAKS(op1.type == JAS_REG_OPERAND_64 || op1.type == JAS_INDIRECT_64, jasGenerateModrm(mode, op2.operand.reg.reg64, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg64 : op1.operand.reg.reg64))
    return JAS_OPERAND_ERROR;

  case 0x12:
  case 0x13:
    WRITE_IF_BREAKS((op2.type == JAS_REG_OPERAND_8 || op2.type == JAS_INDIRECT_8) && op1.type == JAS_REG_OPERAND_8, jasGenerateModrm(mode, op1.operand.reg.reg8, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg8 : op1.operand.reg.reg8))
    WRITE_IF_BREAKS((op2.type == JAS_REG_OPERAND_16 || op2.type == JAS_INDIRECT_16) && op1.type == JAS_REG_OPERAND_16, jasGenerateModrm(mode, op1.operand.reg.reg16, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg16 : op1.operand.reg.reg16))
    WRITE_IF_BREAKS((op2.type == JAS_REG_OPERAND_32 || op2.type == JAS_INDIRECT_32) && op1.type == JAS_REG_OPERAND_32, jasGenerateModrm(mode, op1.operand.reg.reg32, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg32 : op1.operand.reg.reg32))
    WRITE_IF_BREAKS((op2.type == JAS_REG_OPERAND_64 || op2.type == JAS_INDIRECT_64) && op1.type == JAS_REG_OPERAND_64, jasGenerateModrm(mode, op1.operand.reg.reg64, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg64 : op1.operand.reg.reg64))
    return JAS_OPERAND_ERROR;

  default:
    return JAS_OPERAND_ERROR;
  }

  // Old operand encoder:
  // TODO REMOVE LATER!
  // // if (!(instance->buffer[instance->bufferLen - 1] == 0x14 || instance->buffer[instance->bufferLen - 1] == 0x15)) {
  // //   if (op2.type == JAS_OPERAND_8 || op2.type == JAS_OPERAND_16 || op2.type == JAS_OPERAND_32 || op2.type == JAS_OPERAND_64) {
  // //     WRITE_IF(op1.type == JAS_REG_OPERAND_8 || op1.type == JAS_INDIRECT_8, jasGenerateModrm(mode, 2, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg8 : op1.operand.reg.reg8))
  // //     WRITE_IF(op1.type == JAS_REG_OPERAND_16 || op1.type == JAS_INDIRECT_16, jasGenerateModrm(mode, 2, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg16 : op1.operand.reg.reg16))
  // //     WRITE_IF(op1.type == JAS_REG_OPERAND_32 || op1.type == JAS_INDIRECT_32, jasGenerateModrm(mode, 2, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg32 : op1.operand.reg.reg32))
  // //     WRITE_IF(op1.type == JAS_REG_OPERAND_64 || op1.type == JAS_INDIRECT_64, jasGenerateModrm(mode, 2, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg64 : op1.operand.reg.reg64))
  // //   } else {
  // //     WRITE_IF(op1.type == JAS_REG_OPERAND_8 || op1.type == JAS_INDIRECT_8, jasGenerateModrm(mode, 2, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg8 : op1.operand.reg.reg8))
  // //     WRITE_IF(op1.type == JAS_REG_OPERAND_16 || op1.type == JAS_INDIRECT_16, jasGenerateModrm(mode, 2, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg16 : op1.operand.reg.reg16))
  // //     WRITE_IF(op1.type == JAS_REG_OPERAND_32 || op1.type == JAS_INDIRECT_32, jasGenerateModrm(mode, 2, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg32 : op1.operand.reg.reg32))
  // //     WRITE_IF(op1.type == JAS_REG_OPERAND_64 || op1.type == JAS_INDIRECT_64, jasGenerateModrm(mode, 2, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg64 : op1.operand.reg.reg64))
  // //   }
  //// }

  //// ENCODE_IMMEDIATE(op2)
}