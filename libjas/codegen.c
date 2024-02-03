#include "codegen.h"
#include "aaa.h"
#include "aad.h"
#include "aam.h"
#include "aas.h"
#include "adc.h"
#include "add.h"
#include "and.h"
#include "bound.h"
#include "bsf.h"
#include "bsr.h"
#include "bswap.h"
#include "call.h"
#include "cbw.h"
#include "cdqe.h"
#include "clc.h"
#include "cld.h"
#include "clflush.h"
#include "cli.h"
#include "clts.h"
#include "cmc.h"
#include "cmovcc.h"
#include "cwde.h"
#include "error.h"
#include "init.h"
#include "instruction.h"
#include "null.h"
#include <stdint.h>

jasErrorCode_t jasCodegen(jasInstruction_t instruction, jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance) {
  if (instance->buffer == JAS_NULL)
    return JAS_UNDEFINED_POINTER;

  jasInstructionEncoderPointerArray_t jasInstructionEncoder = {&jasAAA, &jasAAD, &jasAAM, &jasAAS, &jasADC, &jasADD, &jasAND, &jasBOUND, &jasBSF, &jasBSR, &jasBSWAP, &jasCALL, &jasCBW, &jasCDQE, &jasCWDE, &jasCLC, &jasCLD, &jasCLFLUSH /* TODO CLFLUSHOPT */, &jasCLI, &jasCLTS, &jasCMC, &jasCMOVA, jasCMOVAE, jasCMOVB, jasCMOVBE, jasCMOVC, jasCMOVE, jasCMOVG, jasCMOVGE, jasCMOVL, jasCMOVLE, jasCMOVNA, jasCMOVNAE, jasCMOVNB, jasCMOVNBE, jasCMOVNC, jasCMOVNE, jasCMOVNG, jasCMOVNGE, jasCMOVNL, jasCMOVNLE, jasCMOVNO, jasCMOVNP, jasCMOVNS, jasCMOVNZ, jasCMOVO, jasCMOVP, jasCMOVPE, jasCMOVPO, jasCMOVS, jasCMOVZ};

  if (instruction > sizeof(jasInstructionEncoder) / sizeof(jasInstructionEncoderPointer_t))
    return JAS_INVALID_INSTRUCTION;

  const jasErrorCode_t errorCode = jasInstructionEncoder[instruction](op1, op2, op3, op4, instance);

  return errorCode;
}
