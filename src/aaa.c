#include "color.h"
#include "write.h"
#include <stdint.h>
#include <stdio.h>

void *aaa(uint8_t *buffer) {
  write8(buffer, 0x37, sizeof(*buffer) / sizeof(uint8_t));
}