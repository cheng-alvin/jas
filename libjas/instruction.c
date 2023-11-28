#include "instruction.h"
#include "null.h"
#include <stdlib.h>
#include <string.h>

#define REPLACE_IF_TRUE(x, y, z) \
  if (strcmp(x, y) == 0)         \
    return z;

instruction_t jasToInstructionEnum(char *instruction) {
  instruction = strlwr(instruction);

  REPLACE_IF_TRUE(instruction, "aaa", AAA);
  REPLACE_IF_TRUE(instruction, "aad", AAD);
}
