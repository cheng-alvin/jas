#include "codegen.h"
#include "error.h"
#include "i.h"
#include "init.h"
#include "macro.h"
#include "mi.h"
#include "modrm.h"
#include "mr.h"
#include "null.h"
#include "operand.h"
#include "remove.h"
#include "rex.h"
#include "rm.h"
#include "write.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static jasErrorCode_t encodeOpcode(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance, jasModrmMode_t *mode, signed long long indexOfRex);

static jasErrorCode_t encodeOperands(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance, jasModrmMode_t mode);

// TODO create new unit tests for `adc`

jasErrorCode_t jasADC(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  if (!jasCheckIfHighRegistersAreValidUnderRexPrefix(op1, op2))
    return JAS_INVALID_HIGH_REGISTER;

  jasModrmMode_t mode = JAS_NULL;

  signed long long indexOfRex = instance->bufferLen;
  CONDITIONAL_WRITE(jasRexExpectedInRegisterEncoding(op1), jasRexConstructPrefix(JAS_NULL, JAS_REX_B))
  else {
    WRITE(0)
  }

  const jasErrorCode_t opcodeExitStatus = encodeOpcode(op1, op2, op3, op4, instance, &mode, indexOfRex);
  if (opcodeExitStatus != JAS_NO_ERROR)
    return opcodeExitStatus;

  if (instance->buffer[indexOfRex] == 0) {
    instance->buffer = removeElement(instance->buffer, instance->bufferLen, indexOfRex);

    instance->bufferLen--;
  }

  return encodeOperands(op1, op2, op3, op4, instance, mode);
}

// TODO if op1 = reg and op2 = r/m then, use 0b00 for mode

static jasErrorCode_t encodeOpcode(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance, jasModrmMode_t *mode, signed long long indexOfRex) {
  switch (op1.type) {
  case JAS_INDIRECT_8:
    *mode = *mode != JAS_NULL ? mode : JAS_MODRM_INDIRECT;
  case JAS_REG_OPERAND_8:
    *mode = *mode != JAS_NULL ? mode : JAS_MODRM_REGISTER;

    WRITE_IF_TRUE_THEN_BREAK(op1.operand.reg.reg8 == JAS_REG_AL, 0x14)

    WRITE_IF_TRUE_THEN_BREAK(op1.type == JAS_REG_OPERAND_8 && (op2.type == JAS_REG_OPERAND_8 || op2.type == JAS_INDIRECT_8), 0x12)
    WRITE_IF_TRUE_THEN_BREAK(op2.type == JAS_REG_OPERAND_8, 0x10)

    WRITE(0x80)
    break;

  case JAS_INDIRECT_16:
    *mode = *mode != JAS_NULL ? mode : JAS_MODRM_INDIRECT;
  case JAS_REG_OPERAND_16:
    *mode = *mode != JAS_NULL ? mode : JAS_MODRM_REGISTER;

    WRITE(OPERAND_SIZE_OVERRIDE)
    WRITE_IF_TRUE_THEN_BREAK(op1.type == JAS_REG_OPERAND_16 && (op2.type == JAS_REG_OPERAND_16 || op2.type == JAS_INDIRECT_16), 0x13)
    WRITE_IF_TRUE_THEN_BREAK(op1.operand.reg.reg16 == JAS_REG_AX, 0x15)
    WRITE_IF_TRUE_THEN_BREAK(op2.type == JAS_OPERAND_8, 0x83)
    WRITE_IF_TRUE_THEN_BREAK(op2.type == JAS_REG_OPERAND_16, 0x11)

    WRITE(0x81)
    break;

  case JAS_INDIRECT_32:
    *mode = *mode != JAS_NULL ? mode : JAS_MODRM_INDIRECT;
  case JAS_REG_OPERAND_32:
    *mode = *mode != JAS_NULL ? mode : JAS_MODRM_REGISTER;

    WRITE_IF_TRUE_THEN_BREAK(op1.type == JAS_REG_OPERAND_32 && (op2.type == JAS_REG_OPERAND_32 || op2.type == JAS_INDIRECT_32), 0x13)
    WRITE_IF_TRUE_THEN_BREAK(op1.operand.reg.reg32 == JAS_REG_EAX, 0x15)
    WRITE_IF_TRUE_THEN_BREAK(op2.type == JAS_OPERAND_8, 0x83)
    WRITE_IF_TRUE_THEN_BREAK(op2.type == JAS_REG_OPERAND_32, 0x11)

    WRITE(0x81)
    break;

  case JAS_INDIRECT_64:
    *mode = *mode != JAS_NULL ? mode : JAS_MODRM_INDIRECT;
  case JAS_REG_OPERAND_64:
    *mode = *mode != JAS_NULL ? mode : JAS_MODRM_REGISTER;

    if (instance->mode != JAS_MODE_64)
      return JAS_LONG_MODE_INSTRUCTION;

    instance->buffer[indexOfRex] = jasRexConstructPrefix(instance->buffer[indexOfRex], JAS_REX_W);

    WRITE_IF_TRUE_THEN_BREAK(op1.type == JAS_REG_OPERAND_64 && (op2.type == JAS_REG_OPERAND_64 || op2.type == JAS_INDIRECT_64), 0x13)
    WRITE_IF_TRUE_THEN_BREAK(op1.operand.reg.reg64 == JAS_REG_RAX, 0x15)
    WRITE_IF_TRUE_THEN_BREAK(op2.type == JAS_OPERAND_8, 0x83)
    WRITE_IF_TRUE_THEN_BREAK(op2.type == JAS_REG_OPERAND_64, 0x11)

    WRITE(0x81)
    break;
  }

  return JAS_NO_ERROR;
}

static jasErrorCode_t encodeOperands(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance, jasModrmMode_t mode) {

  switch (instance->buffer[instance->bufferLen - 1]) {
  case 0x14:
  case 0x15:
    return jasOperandIdentityI(op1, op2, op3, op4, instance);

  case 0x80:
  case 0x81:
  case 0x83:
    return jasExtendedOperandIdentityMI(op1, op2, op3, op4, instance, mode, 2);

  case 0x10:
  case 0x11:
    return jasExtendedOperandIdentityMR(op1, op2, op3, op4, instance, mode, 2);

  case 0x12:
  case 0x13:
    return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, mode, 2);

  default:
    return JAS_OPERAND_ERROR;
  }
}