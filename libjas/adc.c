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
  switch (op1.type) {
  case JAS_MEM_8:
  case JAS_REG_OPERAND_8:
    WRITE_IF_BREAKS(op1.operand.reg.reg8 == JAS_REG_AL, 0x14)
    // TODO REX prefix byte
    // WRITE_IF(jasRexExpectedInRegisterEncoding(op1.operand.reg.reg8),)
    WRITE(0x80)
    break;

  case JAS_MEM_16:
  case JAS_REG_OPERAND_16:
    WRITE_IF_BREAKS(op1.operand.reg.reg16 == JAS_REG_AX, 0x15)
    WRITE(0x81)
    break;

  case JAS_MEM_32:
  case JAS_REG_OPERAND_32:
    WRITE_IF_BREAKS(op1.operand.reg.reg32 == JAS_REG_EAX, 0x15)
    WRITE(0x81)
    break;

  case JAS_MEM_64:
  case JAS_REG_OPERAND_64:
    // TODO REX prefix
    WRITE_IF_BREAKS(op1.operand.reg.reg64 == JAS_REG_RAX, 0x15)
    WRITE(0x81)
    break;
  }
