#include "codegen.h"
#include "aaa.h"
#include "aad.h"
#include "aam.h"
#include "aas.h"
#include "adc.h"
#include "add.h"
#include "and.h"
#include "error.h"
#include "init.h"
#include "instruction.h"
#include "null.h"
#include <stdint.h>

jasErrorCode_t codegen(jasInstruction_t instruction, jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  if (instance->buffer == NULL)
    return JAS_UNDEFINED_POINTER;

  jasInstructionEncoderPointerArray_t jasInstructionEncoder = {&jasAAA, &jasAAD, &jasAAM, &jasAAS, &jasADC, &jasADD, &jasAND};

  if (instruction > sizeof(jasInstructionEncoder) / sizeof(jasInstructionEncoderPointer_t))
    return JAS_INVALID_INSTRUCTION;

  const jasErrorCode_t errorCode = jasInstructionEncoder[instruction](op1, op2, op3, op4, instance);

  return errorCode;
}
