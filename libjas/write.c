#include "write.h"
#include <stdint.h>
#include <stdlib.h>

uint8_t *write8(uint8_t *buffer, uint8_t value, int len) {
  len++;
  buffer = realloc(buffer, len * sizeof(uint8_t));
  buffer[len - 1] = value;
  return buffer;
}

uint8_t *write16(uint8_t *buffer, uint16_t value, int len) {
  buffer = write8(buffer, (uint8_t)value >> 8, len);
  len++;
  buffer = write8(buffer, (uint8_t)value, len);
  return buffer;
}