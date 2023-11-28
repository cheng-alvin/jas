#include "instruction.h"
#include "null.h"
#include <stdlib.h>
#include <string.h>

#define SUBSTITUTE_INSTRUCTION_IF_EQUATES(x, y, z) \
  if (strcmp(x, y) == 0)                           \
    return z;

instruction_t jasToInstructionEnum(char *instruction) {
  instruction = strlwr(instruction);

  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "aaa", AAA);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "aad", AAD);
}
