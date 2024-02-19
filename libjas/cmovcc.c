#include "codegen.h"
#include "error.h"
#include "init.h"
#include "macro.h"
#include "null.h"
#include "rex.h"
#include "rm.h"
#include "write.h"
#include <stdint.h>

static jasErrorCode_t generateRex(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance);

static jasErrorCode_t initCMOVccInstructionGroup(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance);

jasErrorCode_t jasCMOVA(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x47)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVAE(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x43)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVB(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x42)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVBE(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x46)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVC(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x42)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVE(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x44)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVG(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x4F)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVGE(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x4D)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVL(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x4C)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVLE(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x4E)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVNA(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x46)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVNAE(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x42)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVNB(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x43)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVNBE(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x47)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVNC(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x43)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVNE(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x45)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVNG(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x4E)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVNGE(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x4C)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVNL(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x4D)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVNLE(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x4F)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVNO(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x41)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVNP(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x4B)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVNS(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x49)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVNZ(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x45)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVO(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x40)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVP(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x4A)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVPE(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x4A)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVPO(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x4B)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVS(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x48)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

jasErrorCode_t jasCMOVZ(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t initErrorCode = initCMOVccInstructionGroup(op1, op2, op3, op4, instance);
  if (initErrorCode != JAS_NO_ERROR)
    return initErrorCode;

  WRITE(0x44)

  return jasExtendedOperandIdentityRM(op1, op2, op3, op4, instance, JAS_MODRM_REGISTER_OPERAND, 0x0);
}

//! -- Helper static functions --

static jasErrorCode_t initCMOVccInstructionGroup(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  const jasErrorCode_t rexExitStatus = generateRex(op1, op2, op3, op4, instance);
  if (rexExitStatus != JAS_NO_ERROR)
    return rexExitStatus;

  WRITE(0x0F)
}

static jasErrorCode_t generateRex(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  uint8_t prefix;

  if (!jasCheckIfHighRegistersAreValidUnderRexPrefix(op1, op2))
    return JAS_INVALID_HIGH_REGISTER;

  if (jasRexExpectedInRegisterEncoding(op1))
    prefix = jasRexConstructPrefix(prefix, JAS_REX_B);

  if (jasRexExpectedInRegisterEncoding(op2))
    prefix = jasRexConstructPrefix(prefix, JAS_REX_B);

  if (op1.type == JAS_REG_OPERAND_64) {
    prefix = jasRexConstructPrefix(prefix, JAS_REX_W);

    if (instance->mode != JAS_MODE_64)
      return JAS_LONG_MODE_INSTRUCTION;
  }

  if (prefix == 0)
    return JAS_INTERNAL_ERROR;

  WRITE(prefix)
  return JAS_NO_ERROR;
}