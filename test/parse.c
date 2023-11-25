#include "parse.h"
#include "color.h"
#include "labrador.h"
#include <stdlib.h>
#include <string.h>

#define EXPECTED_INSTRUCTION "abc"
#define EXPECTED_ARG1 "x"
#define EXPECTED_ARG2 "y"
#define EXPECTED_ARG3 "z"

TEST(parse) {
  const char src[] = "abc x, y, z\n";
  instruction_t **instructions = parse(src);
  SHOULD_EQUAL(strcmp(instructions[0]->instruction, EXPECTED_INSTRUCTION), 0)
  SHOULD_EQUAL(strcmp(instructions[0]->arg1, EXPECTED_ARG1), 0)
  SHOULD_EQUAL(strcmp(instructions[0]->arg2, EXPECTED_ARG2), 0)
  SHOULD_EQUAL(strcmp(instructions[0]->arg3, EXPECTED_ARG3), 1)

  free(instructions[0]);
  free(instructions);

  PASS
}