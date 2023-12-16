#include "operand.h"
#include "size.h"
#include <stdbool.h>

bool jasRexExpectedInRegisterEncoding(jasRegOperandType_t reg) {
  switch (jasRegisterSize(&(jasOperand_t){.reg = reg})) {
  case 8:
    switch (reg.reg8) {
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

  case 16:
    switch (reg.reg16) {
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

  case 32:
    switch (reg.reg32) {
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

  case 64:
    switch (reg.reg64) {
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

  // TODO document undefined behavior
  return false;
}
