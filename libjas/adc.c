#include "codegen.h"
#include "error.h"
#include "init.h"
#include "macro.h"
#include "modrm.h"
#include "null.h"
#include "operand.h"
#include "write.h"
#include <stdint.h>

jasErrorCode_t adc(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  switch (op1.type) {
  case JAS_REG_OPERAND_8:
    WRITE_IF(op1.operand.reg.reg8 == JAS_REG_AL, 0x14)

  case JAS_REG_OPERAND_16:
    WRITE_IF(op1.operand.reg.reg16 == JAS_REG_AX, 0x15)

  case JAS_REG_OPERAND_32:
    WRITE_IF(op1.operand.reg.reg32 == JAS_REG_EAX, 0x15)

  case JAS_REG_OPERAND_64:
    // TODO REX prefix
    WRITE_IF(op1.operand.reg.reg64 == JAS_REG_RAX, 0x15)
  }
