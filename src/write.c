#include "write.h"
#include <stdbool.h>
#include <stdint.h>

uint8_t *write8(uint8_t *buffer, uint8_t value) {
  const int len = *buffer / sizeof(uint8_t);
  buffer = realloc(buffer, len + sizeof(uint8_t));

  buffer[len] = value;

  return buffer;
}

uint8_t *write16(uint8_t *buffer, uint16_t value) {
  const int len = *buffer / sizeof(uint16_t);
  buffer = realloc(buffer, len + sizeof(uint16_t));

  value = little_endian16(value);
  buffer[len] = value;

  return buffer;
}