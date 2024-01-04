#include "operand.h"
#include "error.h"
#include <stdbool.h>

bool jasRexExpectedInRegisterEncoding(jasTaggedOperand_t x) {
  switch (x.type) {
  case JAS_REG_OPERAND_8:
    switch (x.operand.reg.reg8) {
    case JAS_REG_R8B:
    case JAS_REG_R9B:
    case JAS_REG_R10B:
    case JAS_REG_R11B:
    case JAS_REG_R12B:
    case JAS_REG_R13B:
    case JAS_REG_R14B:
    case JAS_REG_R15B:
    case JAS_REG_SIL:
    case JAS_REG_DIL:
    case JAS_REG_SPL:
    case JAS_REG_BPL:
      return true;
    default:
      return false;
    }

  case JAS_REG_OPERAND_16:
    switch (x.operand.reg.reg16) {
    case JAS_REG_R8W:
    case JAS_REG_R9W:
    case JAS_REG_R10W:
    case JAS_REG_R11W:
    case JAS_REG_R12W:
    case JAS_REG_R13W:
    case JAS_REG_R14W:
    case JAS_REG_R15W:
      return true;
    default:
      return false;
    }

  case JAS_REG_OPERAND_32:
    switch (x.operand.reg.reg32) {
    case JAS_REG_R8D:
    case JAS_REG_R9D:
    case JAS_REG_R10D:
    case JAS_REG_R11D:
    case JAS_REG_R12D:
    case JAS_REG_R13D:
    case JAS_REG_R14D:
    case JAS_REG_R15D:
      return true;
    default:
      return false;
    }

  case JAS_REG_OPERAND_64:
    switch (x.operand.reg.reg64) {
    case JAS_REG_R8:
    case JAS_REG_R9:
    case JAS_REG_R10:
    case JAS_REG_R11:
    case JAS_REG_R12:
    case JAS_REG_R13:
    case JAS_REG_R14:
    case JAS_REG_R15:
      return true;
    default:
      return false;
    }
  }

  return false;
}

jasTaggedOperand_t jasConstructOperand(void *value, jasOperandType_t type) {
  jasUntaggedOperand_t revisedValue;

  switch (type) {
  case JAS_REG_OPERAND_8:
    revisedValue.reg.reg8 = *(jasReg8_t *)value;
    break;

  case JAS_REG_OPERAND_16:
    revisedValue.reg.reg16 = *(jasReg16_t *)value;
    break;

  case JAS_REG_OPERAND_32:
    revisedValue.reg.reg32 = *(jasReg32_t *)value;
    break;

  case JAS_REG_OPERAND_64:
    revisedValue.reg.reg64 = *(jasReg64_t *)value;
    break;

  case JAS_OPERAND_8:
    revisedValue.operand8 = *(uint8_t *)value;
    break;

  case JAS_OPERAND_16:
    revisedValue.operand16 = *(uint16_t *)value;
    break;

  case JAS_OPERAND_32:
    revisedValue.operand32 = *(uint32_t *)value;
    break;

  case JAS_OPERAND_64:
    revisedValue.operand64 = *(uint64_t *)value;
    break;

  default:
    return JAS_NO_OPERAND;
  }

  return (jasTaggedOperand_t){.type = type, .operand = revisedValue};
}

bool jasCheckIfHighRegistersAreValidUnderRexPrefix(jasTaggedOperand_t op1, jasTaggedOperand_t op2) {
  if (jasRexExpectedInRegisterEncoding(op1) || jasRexExpectedInRegisterEncoding(op2)) {
    return false;
  }
  return true;
}

uint8_t jasGetRegField(uint8_t x) {
  const uint8_t lookupTable[] = {0, 1, 2, 3, 4, 5, 6, 7, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7};

  return lookupTable[x];
}