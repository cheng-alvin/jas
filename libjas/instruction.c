#include "instruction.h"
#include "null.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define SUBSTITUTE_INSTRUCTION_IF_EQUATES(x, y, z) \
  if (strcmp(x, y) == 0)                           \
    return z;

// TODO Implement a checker for checking if the enum actually exists.

jasInstruction_t jasToInstructionEnum(char *instruction) {
  for (; *instruction; ++instruction)
    *instruction = tolower(*instruction);

  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "aaa", AAA);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "aad", AAD);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "aam", AAM);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "aas", AAS);

  return -1;
}
