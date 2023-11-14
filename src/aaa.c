#include "color.h"
#include "write.h"
#include <stdint.h>
#include <stdio.h>

uint8_t *aaa(uint8_t *buffer) {
  return write8(buffer, 0x37, sizeof(*buffer) / sizeof(uint8_t));
}