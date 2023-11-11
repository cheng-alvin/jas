#include "codegen.h"
#include "aaa.h"
#include "color.h"
#include "null.h"
#include "parse.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO Refactor error reporting

uint8_t *codegen(instruction_t *arr, int len, mode_t mode, char *file) {
  uint8_t *buffer = malloc(sizeof(uint8_t) * 15 * len);
  int error = 0;

  for (int i = 0; i < len; i++) {
    int line = i + 1;
    if (!strcmp(arr[i].instruction, "aaa")) {
      if (mode == LONG) {
        fprintf(stderr, BOLD "%s:%d: %serror:%s `aaa` instruction not supported in long-mode\n" RESET, file, line, RED, RESET);
        error++;
      }

      aaa(buffer);
    }

    else {
      fprintf(stderr, BOLD "%s:%d: %serror:%s `%s` is NOT a supported instruction.\n" RESET, file, line, RED, RESET, arr[i].instruction);
      error++;
    }
  }

  return error ? NULL : buffer;
}