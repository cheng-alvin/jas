#include "codegen.h"
#include "error.h"
#include "init.h"
#include "macro.h"
#include "mi.h"
#include "modrm.h"
#include "mr.h"
#include "null.h"
#include "operand.h"
#include "rex.h"
#include "rm.h"
#include "write.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static void encodeOpcode(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance, jasModrmMode_t mode, signed long long indexOfRex);

static jasErrorCode_t encodeOperands(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance, jasModrmMode_t mode);

// TODO Maybe rename WRITE_IF_TRUE_THEN_BREAK() macro (this name sounds absolutely awful))

jasErrorCode_t jasAND(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  if (jasCheckIfHighRegistersAreValidUnderRexPrefix(op1, op2))
    return JAS_INVALID_HIGH_REGISTER;

  jasModrmMode_t mode;

  // TODO encapsulate behaviour
  // Note: If there's no REX prefix, therefore the `indexOfRex` will be 0 >
  signed long long indexOfRex = jasRexExpectedInRegisterEncoding(op1) ? instance->bufferLen : -1;
  CONDITIONAL_WRITE(jasRexExpectedInRegisterEncoding(op1), jasRexConstructPrefix(NULL, JAS_REX_B))

  encodeOpcode(op1, op2, op3, op4, instance, mode, indexOfRex);
  return encodeOperands(op1, op2, op3, op4, instance, mode);
}

static void encodeOpcode(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance, jasModrmMode_t mode, signed long long indexOfRex) {
  switch (op1.type) {
  case JAS_INDIRECT_8:
    mode = JAS_MODRM_INDIRECT;
  case JAS_REG_OPERAND_8:
    mode = JAS_MODRM_REGISTER;

    WRITE_IF_TRUE_THEN_BREAK(op1.operand.reg.reg8 == JAS_REG_AL, 0x24)

    WRITE_IF_TRUE_THEN_BREAK(op1.type == JAS_REG_OPERAND_8 && (op2.type == JAS_REG_OPERAND_8 || op2.type == JAS_INDIRECT_8), 0x22)
    WRITE_IF_TRUE_THEN_BREAK(op2.type == JAS_REG_OPERAND_8, 0x20)

    WRITE(0x80)
    break;

  case JAS_INDIRECT_16:
    mode = JAS_MODRM_INDIRECT;
  case JAS_REG_OPERAND_16:
    mode = JAS_MODRM_REGISTER;

    WRITE(OPERAND_SIZE_OVERRIDE)
    WRITE_IF_TRUE_THEN_BREAK(op1.type == JAS_REG_OPERAND_16 && (op2.type == JAS_REG_OPERAND_16 || op2.type == JAS_INDIRECT_16), 0x23)
    WRITE_IF_TRUE_THEN_BREAK(op1.operand.reg.reg16 == JAS_REG_AX, 0x25)
    WRITE_IF_TRUE_THEN_BREAK(op2.type == JAS_OPERAND_8, 0x83)
    WRITE_IF_TRUE_THEN_BREAK(op2.type == JAS_REG_OPERAND_16, 0x21)

    WRITE(0x81)
    break;

  case JAS_INDIRECT_32:
    mode = JAS_MODRM_INDIRECT;
  case JAS_REG_OPERAND_32:
    mode = JAS_MODRM_REGISTER;

    WRITE_IF_TRUE_THEN_BREAK(op1.type == JAS_REG_OPERAND_32 && (op2.type == JAS_REG_OPERAND_32 || op2.type == JAS_INDIRECT_32), 0x23)
    WRITE_IF_TRUE_THEN_BREAK(op1.operand.reg.reg32 == JAS_REG_EAX, 0x25)
    WRITE_IF_TRUE_THEN_BREAK(op2.type == JAS_OPERAND_8, 0x83)
    WRITE_IF_TRUE_THEN_BREAK(op2.type == JAS_REG_OPERAND_32, 0x21)

    WRITE(0x81)
    break;

  case JAS_INDIRECT_64:
    mode = JAS_MODRM_INDIRECT;
  case JAS_REG_OPERAND_64:
    mode = JAS_MODRM_REGISTER;

    NO_LONG_MODE

    WRITE(jasRexConstructPrefix(indexOfRex > 0 ? instance->buffer[indexOfRex] : NULL, JAS_REX_W))

    WRITE_IF_TRUE_THEN_BREAK(op1.type == JAS_REG_OPERAND_64 && (op2.type == JAS_REG_OPERAND_64 || op2.type == JAS_INDIRECT_64), 0x23)
    WRITE_IF_TRUE_THEN_BREAK(op1.operand.reg.reg64 == JAS_REG_RAX, 0x25)
    WRITE_IF_TRUE_THEN_BREAK(op2.type == JAS_OPERAND_8, 0x83)
    WRITE_IF_TRUE_THEN_BREAK(op2.type == JAS_REG_OPERAND_64, 0x21)

    WRITE(0x81)
    break;
  }
}

static jasErrorCode_t encodeOperands(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance, jasModrmMode_t mode) {
  // TODO encapsulate behaviour - Which is stupid how I did not encapsulate this behaviour in the first place.
  // Operand encoder:
  switch (instance->buffer[instance->bufferLen - 1]) {
  case 0x24:
  case 0x25:
    return jasOperandIdentityI(op1, op2, op3, op4, instance);

  case 0x80:
  case 0x81:
  case 0x83:
    return jasExtendedOperandIdentityMI(op1, op2, op3, op4, instance, mode, 4);

  case 0x20:
  case 0x21:
    return jasExtendedOperandIdentityMR(op1, op2, op3, op4, instance, mode, 4);

  case 0x22:
  case 0x23:
    return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, mode, 4);

  default:
    return JAS_OPERAND_ERROR;
  }
}