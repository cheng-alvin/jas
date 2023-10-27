#include "parse.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  char in[] = "mov rax, 0x60 \nmov rax, 0x60";
  instruction_t **lines = parse(in);

  free(lines);

  return 0;
}