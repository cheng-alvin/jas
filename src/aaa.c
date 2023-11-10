#include "color.h"
#include "debug_info.h"
#include "write.h"
#include <stdint.h>
#include <stdio.h>

uint8_t *aaa(uint8_t *buffer) {
  return write8(buffer, 0x37, sizeof(*buffer) / sizeof(uint8_t));

  // TODO Migrate to be independent!
  // fprintf(stderr, BOLD "%s:%d: %serror:%s `aaa` instruction not supported in long-mode\n" RESET, info->file, info->line_number, RED, RESET);
}