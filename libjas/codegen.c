#include "codegen.h"
#include "aaa.h"
#include "aad.h"
#include "aam.h"
#include "aas.h"
#include "error.h"
#include "init.h"
#include "instruction.h"
#include "null.h"
#include <stdint.h>

jasErrorCode_t codegen(jasInstruction_t instruction, jasOperand_t op1, jasOperand_t op2, jasOperand_t op3, jasOperand_t op4, jasInstance_t instance) {
  if (instance.buffer == NULL)
    return JAS_UNDEFINED_POINTER;

  jasInstructionEncoderPointerArray_t jasInstructionEncoder = {&aaa, &aad, &aam, &aas};

  if (instruction > sizeof(jasInstructionEncoder) / sizeof(&aaa))
    return JAS_INVALID_INSTRUCTION;

  const jasErrorCode_t errorCode = jasInstructionEncoder[instruction](op1, op2, op3, op4, instance);

  return errorCode;
}
