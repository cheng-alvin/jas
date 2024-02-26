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

// I could not find much documentation for the 0xFF and 0x9A far
// calls. Therefore, I will be not supporting these flavours of
// the call instruction. Until more sources can be found, the
// encoder will return an operand error.

//! Note: Usage of opcodes 0xFF and 0x9A is not supported.

static jasErrorCode_t encodeOpcode(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance, jasModrmMode_t *mode);

jasErrorCode_t jasCALL(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  jasModrmMode_t mode = JAS_NULL;

  const jasErrorCode_t opcodeExitStatus = encodeOpcode(op1, op2, op3, op4, instance, &mode);
  if (opcodeExitStatus != JAS_NO_ERROR)
    return opcodeExitStatus;

  if (instance->buffer[instance->bufferLen - 1] == 0xE8)
    ENCODE_IMMEDIATE(op1)
  else
    jasExtendedOperandIdentityMI(op1, op2, op3, op4, instance, mode, 2);

  return JAS_NO_ERROR;
}

static jasErrorCode_t encodeOpcode(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance, jasModrmMode_t *mode) {
  switch (op1.type) {
    // TODO Make separate types for what intel refers to as `rel16` and `ptr16:16` pointers?!?!
    // Defaulting to `JAS_OPERAND_16` `JAS_OPERAND_32` for 16 and
    // 32 bits of data. respectively.

  case JAS_OPERAND_32:
  case JAS_OPERAND_16:
    WRITE(0xE8)
    break;

  case JAS_REG_OPERAND_16:
  case JAS_REG_OPERAND_32:
  case JAS_REG_OPERAND_64:
    *mode = *mode != JAS_NULL ? mode : JAS_MODRM_REGISTER;

  case JAS_INDIRECT_16:
  case JAS_INDIRECT_32:
  case JAS_INDIRECT_64:
    *mode = *mode != JAS_NULL ? mode : JAS_MODRM_INDIRECT;

    if (op1.type == JAS_REG_OPERAND_64 || op1.type == JAS_INDIRECT_64) {
      if (instance->mode != JAS_MODE_32)
        return JAS_LONG_MODE_INSTRUCTION;
    } else {
      if (instance->mode == JAS_MODE_64)
        return JAS_NON_LONG_MODE_INSTRUCTION;
    }

    WRITE(0xFF)
    break;

  default:
    return JAS_OPERAND_ERROR;
  }

  return JAS_NO_ERROR;
}
