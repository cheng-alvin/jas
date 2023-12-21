#include "codegen.h"
#include "error.h"
#include "init.h"
#include "macro.h"
#include "modrm.h"
#include "null.h"
#include "operand.h"
#include "write.h"
#include <stdint.h>

// TODO Maybe rename WRITE_IF_BREAKS() macro (this name sounds absolutly awful))

jasErrorCode_t adc(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  jasModrmMode_t mode;

  switch (op1.type) {
  case JAS_INDIRECT_8:
    mode = JAS_MODRM_INDIRECT;
  case JAS_REG_OPERAND_8:
    mode = JAS_MODRM_REGISTER;

    WRITE_IF_BREAKS(op1.operand.reg.reg8 == JAS_REG_AL, 0x14)

    // TODO REX prefix byte
    // WRITE_IF(jasRexExpectedInRegisterEncoding(op1.operand.reg.reg8),)

    WRITE_IF_BREAKS(op1.type == JAS_REG_OPERAND_8 && (op2.type == JAS_REG_OPERAND_8 || op2.type == JAS_INDIRECT_8), 0x12)
    WRITE_IF_BREAKS(op2.type == JAS_REG_OPERAND_8, 0x10)

    WRITE(0x80)
    break;

  case JAS_INDIRECT_16:
    mode = JAS_MODRM_INDIRECT;
  case JAS_REG_OPERAND_16:
    mode = JAS_MODRM_REGISTER;

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

    // TODO REX prefix
    WRITE_IF_BREAKS(op1.type == JAS_REG_OPERAND_64 && (op2.type == JAS_REG_OPERAND_64 || op2.type == JAS_INDIRECT_64), 0x13)
    WRITE_IF_BREAKS(op1.operand.reg.reg64 == JAS_REG_RAX, 0x15)
    WRITE_IF_BREAKS(op2.type == JAS_OPERAND_8, 0x83)
    WRITE_IF_BREAKS(op2.type == JAS_REG_OPERAND_64, 0x11)

    WRITE(0x81)
    break;
  }

  switch (op2.type) {
  case JAS_OPERAND_8:
    WRITE(op2.operand.operand8)
    NO_ERROR

  case JAS_OPERAND_16:
  }
}