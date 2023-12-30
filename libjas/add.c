// Basically just a copy if the `adc` function.
// Therefore, this shall be tested to ensure that there are no errors for opcodes while being transferred over to the `add` function.

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

// TODO Maybe rename BREAKABLE_CONDITIONAL_WRITE() macro (this name sounds absolutely awful))

jasErrorCode_t add(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  if (jasCheckIfHighRegistersAreValidUnderRexPrefix(op1, op2))
    return JAS_INVALID_HIGH_REGISTER;

  jasModrmMode_t mode;

  // TODO encapsulate behaviour
  // Note: If there's no REX prefix, therefore the `indexOfRex` will be 0 >
  signed long long indexOfRex = jasRexExpectedInRegisterEncoding(op1) ? instance->bufferLen : -1;
  CONDITIONAL_WRITE(jasRexExpectedInRegisterEncoding(op1), jasRexConstructPrefix(NULL, JAS_REX_B))

  switch (op1.type) {
  case JAS_INDIRECT_8:
    mode = JAS_MODRM_INDIRECT;
  case JAS_REG_OPERAND_8:
    mode = JAS_MODRM_REGISTER;

    BREAKABLE_CONDITIONAL_WRITE(op1.operand.reg.reg8 == JAS_REG_AL, 0x04)

    BREAKABLE_CONDITIONAL_WRITE(op1.type == JAS_REG_OPERAND_8 && (op2.type == JAS_REG_OPERAND_8 || op2.type == JAS_INDIRECT_8), 0x02)
    BREAKABLE_CONDITIONAL_WRITE(op2.type == JAS_REG_OPERAND_8, 0x00)

    WRITE(0x80)
    break;

  case JAS_INDIRECT_16:
    mode = JAS_MODRM_INDIRECT;
  case JAS_REG_OPERAND_16:
    mode = JAS_MODRM_REGISTER;

    OPERAND_SIZE_OVERRIDE16
    BREAKABLE_CONDITIONAL_WRITE(op1.type == JAS_REG_OPERAND_16 && (op2.type == JAS_REG_OPERAND_16 || op2.type == JAS_INDIRECT_16), 0x03)
    BREAKABLE_CONDITIONAL_WRITE(op1.operand.reg.reg16 == JAS_REG_AX, 0x05)
    BREAKABLE_CONDITIONAL_WRITE(op2.type == JAS_OPERAND_8, 0x83)
    BREAKABLE_CONDITIONAL_WRITE(op2.type == JAS_REG_OPERAND_16, 0x01)

    WRITE(0x81)
    break;

  case JAS_INDIRECT_32:
    mode = JAS_MODRM_INDIRECT;
  case JAS_REG_OPERAND_32:
    mode = JAS_MODRM_REGISTER;

    BREAKABLE_CONDITIONAL_WRITE(op1.type == JAS_REG_OPERAND_32 && (op2.type == JAS_REG_OPERAND_32 || op2.type == JAS_INDIRECT_32), 0x03)
    BREAKABLE_CONDITIONAL_WRITE(op1.operand.reg.reg32 == JAS_REG_EAX, 0x05)
    BREAKABLE_CONDITIONAL_WRITE(op2.type == JAS_OPERAND_8, 0x83)
    BREAKABLE_CONDITIONAL_WRITE(op2.type == JAS_REG_OPERAND_32, 0x01)

    WRITE(0x81)
    break;

  case JAS_INDIRECT_64:
    mode = JAS_MODRM_INDIRECT;
  case JAS_REG_OPERAND_64:
    mode = JAS_MODRM_REGISTER;

    NO_LONG_MODE

    // TODO Encapsulate behaviour
    CONDITIONAL_WRITE(indexOfRex < 0, jasRexConstructPrefix(NULL, JAS_REX_W))
    else WRITE(jasRexConstructPrefix(instance->buffer[indexOfRex], JAS_REX_W));

    BREAKABLE_CONDITIONAL_WRITE(op1.type == JAS_REG_OPERAND_64 && (op2.type == JAS_REG_OPERAND_64 || op2.type == JAS_INDIRECT_64), 0x03)
    BREAKABLE_CONDITIONAL_WRITE(op1.operand.reg.reg64 == JAS_REG_RAX, 0x05)
    BREAKABLE_CONDITIONAL_WRITE(op2.type == JAS_OPERAND_8, 0x83)
    BREAKABLE_CONDITIONAL_WRITE(op2.type == JAS_REG_OPERAND_64, 0x01)

    WRITE(0x81)
    break;
  }

  // TODO encapsulate behaviour - Which is stupid how I did not encapsulate this behaviour in the first place.
  // Operand encoder:
  switch (instance->buffer[instance->bufferLen - 1]) {
  case 0x04:
  case 0x05:
    ENCODE_IMMEDIATE(op2)
    return JAS_OPERAND_ERROR;

  case 0x80:
  case 0x81:
  case 0x83:
    CONDITIONAL_WRITE(op1.type == JAS_REG_OPERAND_8 || op1.type == JAS_INDIRECT_8, jasGenerateModrm(mode, 2, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg8 : op1.operand.reg.reg8))
    CONDITIONAL_WRITE(op1.type == JAS_REG_OPERAND_16 || op1.type == JAS_INDIRECT_16, jasGenerateModrm(mode, 2, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg16 : op1.operand.reg.reg16))
    CONDITIONAL_WRITE(op1.type == JAS_REG_OPERAND_32 || op1.type == JAS_INDIRECT_32, jasGenerateModrm(mode, 2, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg32 : op1.operand.reg.reg32))
    CONDITIONAL_WRITE(op1.type == JAS_REG_OPERAND_64 || op1.type == JAS_INDIRECT_64, jasGenerateModrm(mode, 2, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg64 : op1.operand.reg.reg64))
    ENCODE_IMMEDIATE(op2)
    return JAS_OPERAND_ERROR;

  case 0x00:
  case 0x01:
    BREAKABLE_CONDITIONAL_WRITE(op1.type == JAS_REG_OPERAND_8 || op1.type == JAS_INDIRECT_8, jasGenerateModrm(mode, op2.operand.reg.reg8, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg8 : op1.operand.reg.reg8))
    BREAKABLE_CONDITIONAL_WRITE(op1.type == JAS_REG_OPERAND_16 || op1.type == JAS_INDIRECT_16, jasGenerateModrm(mode, op2.operand.reg.reg16, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg16 : op1.operand.reg.reg16))
    BREAKABLE_CONDITIONAL_WRITE(op1.type == JAS_REG_OPERAND_32 || op1.type == JAS_INDIRECT_32, jasGenerateModrm(mode, op2.operand.reg.reg32, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg32 : op1.operand.reg.reg32))
    BREAKABLE_CONDITIONAL_WRITE(op1.type == JAS_REG_OPERAND_64 || op1.type == JAS_INDIRECT_64, jasGenerateModrm(mode, op2.operand.reg.reg64, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg64 : op1.operand.reg.reg64))
    return JAS_OPERAND_ERROR;

  case 0x02:
  case 0x03:
    BREAKABLE_CONDITIONAL_WRITE((op2.type == JAS_REG_OPERAND_8 || op2.type == JAS_INDIRECT_8) && op1.type == JAS_REG_OPERAND_8, jasGenerateModrm(mode, op1.operand.reg.reg8, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg8 : op1.operand.reg.reg8))
    BREAKABLE_CONDITIONAL_WRITE((op2.type == JAS_REG_OPERAND_16 || op2.type == JAS_INDIRECT_16) && op1.type == JAS_REG_OPERAND_16, jasGenerateModrm(mode, op1.operand.reg.reg16, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg16 : op1.operand.reg.reg16))
    BREAKABLE_CONDITIONAL_WRITE((op2.type == JAS_REG_OPERAND_32 || op2.type == JAS_INDIRECT_32) && op1.type == JAS_REG_OPERAND_32, jasGenerateModrm(mode, op1.operand.reg.reg32, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg32 : op1.operand.reg.reg32))
    BREAKABLE_CONDITIONAL_WRITE((op2.type == JAS_REG_OPERAND_64 || op2.type == JAS_INDIRECT_64) && op1.type == JAS_REG_OPERAND_64, jasGenerateModrm(mode, op1.operand.reg.reg64, mode == JAS_MODRM_INDIRECT ? op1.operand.reg.indirectReg64 : op1.operand.reg.reg64))
    return JAS_OPERAND_ERROR;

  default:
    return JAS_OPERAND_ERROR;
  }
}