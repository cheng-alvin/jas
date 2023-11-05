#include "color.h"
#include "constants.h"
#include "write.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t *aaa(uint8_t *buffer, info_t *info) {
  if (info->mode != LONG)
    return write8(buffer, 0x37, sizeof(*buffer) / sizeof(uint8_t));

  fprintf(stderr, BOLD "%s:%d: %serror:%s `aaa` instruction not supported in long-mode\n" RESET, info->file, info->line_number, RED, RESET);

  return NULL;
}