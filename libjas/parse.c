#include "parse.h"
#include "null.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OPERAND_SEP ", "

/**
 * @deprecated
 */

instruction_t **parse(char *buffer) {
  char **lines = (char **)malloc(sizeof(char *) * 8);

  int i = 0;
  char *line = strtok(buffer, "\n");
  while (line != JAS_NULL) {
    if (i == sizeof(lines) / sizeof(char *)) {
      lines = (char **)realloc(lines, sizeof(char *) * 2 * (i + 1));
    }

    lines[i] = line;
    line = strtok(JAS_NULL, "\n");
    i++;
  }

  instruction_t **instructions =
      (instruction_t **)malloc(sizeof(instruction_t *) * (i + 1));

  for (int j = 0; j < i; j++) {
    char *currentLine = lines[j];

    instructions[j] = (instruction_t *)malloc(sizeof(instruction_t));
    instructions[j]->instruction = strtok(currentLine, " ");
    instructions[j]->arg1 = strtok(JAS_NULL, OPERAND_SEP);
    instructions[j]->arg2 = strtok(JAS_NULL, OPERAND_SEP);
    instructions[j]->arg3 = strtok(JAS_NULL, OPERAND_SEP);
  }

  free(lines);

  return instructions;
}
