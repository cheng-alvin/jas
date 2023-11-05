#include "constants.h"
#include <stdint.h>

uint8_t aaa(uint8_t *buffer, modes_t mode) {
  if (mode != LONG)
    buffer = write8(buffer, 0x37, sizeof(*buffer) / sizeof(uint8_t));

  // TODO Error handle

  return buffer;
}