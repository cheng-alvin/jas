#include <stdint.h>
#include <stdlib.h>

uint8_t *write8(uint8_t *buffer, uint8_t value) {
  const int len = *buffer / sizeof(uint8_t);
  buffer = realloc(buffer, len + sizeof(uint8_t));

  buffer[len] = value;

  return buffer;
}