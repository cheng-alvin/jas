#include "color.h"
#include "labrador.h"
#include "parse.h"
#include <stdlib.h>
#include <string.h>

#define EXPECTED_INSTRUCTION "abc"
#define EXPECTED_ARG1 "x"
#define EXPECTED_ARG2 "y"
#define EXPECTED_ARG3 "z"
#define SOURCE "abc x, y, z\n"

TEST(PARSE) {
  instruction_t **instructions = parse(SOURCE);
  SHOULD_EQUAL(strcmp(instructions[0]->instruction, EXPECTED_INSTRUCTION), 0)
  SHOULD_EQUAL(strcmp(instructions[0]->arg1, EXPECTED_ARG1), 0)
  SHOULD_EQUAL(strcmp(instructions[0]->arg2, EXPECTED_ARG2), 0)
  SHOULD_EQUAL(strcmp(instructions[0]->arg3, EXPECTED_ARG3), 0)

  free(instructions[0]);
  free(instructions);

  PASS
}