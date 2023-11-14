#include "color.h"
#include "write.h"
#include <stdint.h>
#include <stdio.h>

uint8_t *JAS_aaa(uint8_t *buffer) {
  return JAS_write_8(buffer, 0x37, sizeof(*buffer) / sizeof(uint8_t));
}