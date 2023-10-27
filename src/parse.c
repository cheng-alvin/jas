#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **parse(char *buffer) {
  char **lines = (char **)malloc(sizeof(char *) * 8);

  int i = 0;
  char *line = strtok(buffer, "\n");
  while (line != NULL) {
    if (i == sizeof(lines) / sizeof(char *)) {
      lines = (char **)realloc(lines, sizeof(char *) * 2 * (i + 1));
    }

    lines[i] = line;
    printf("%s\n", lines[i]);
    line = strtok(NULL, "\n");
    i++;
  }

  return lines;
}
