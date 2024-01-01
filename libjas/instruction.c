#include "instruction.h"
#include "null.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define SUBSTITUTE_INSTRUCTION_IF_EQUATES(x, y, z) \
  if (strcmp(x, y) == 0)                           \
    return z;

jasInstruction_t jasToInstructionEnum(char *instruction) {
  for (; *instruction; ++instruction)
    *instruction = tolower(*instruction);

  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "aaa", AAA);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "aad", AAD);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "aam", AAM);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "aas", AAS);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "adc", ADC);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "add", ADD);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "and", AND);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "bound", BOUND);
  return -1;
}
