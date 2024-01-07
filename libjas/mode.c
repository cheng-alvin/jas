#include "modrm.h"
#include "operand.h"

jasModrmMode_t jasGetMode(jasTaggedOperand_t op1, jasTaggedOperand_t op2) {
  switch (op1.type) {
  case JAS_REG_OPERAND_64:
  case JAS_REG_OPERAND_32:
  case JAS_REG_OPERAND_16:
    return JAS_MODRM_REGISTER;

  case JAS_INDIRECT_64:
  case JAS_INDIRECT_32:
  case JAS_INDIRECT_16:
    return JAS_MODRM_INDIRECT;
  }
}