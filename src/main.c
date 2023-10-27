#include "parse.h"
#include <stdio.h>

int main(int argc, char **argv) {
  char in[] = "mov rax, 0x60 \nmov rax, 0x60";
  char **lines = parse(in);
  return 0;
}