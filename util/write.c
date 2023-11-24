#include "write.h"
#include "endian.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t *write8(uint8_t *buffer, uint8_t value, int len) {
  buffer = realloc(buffer, len + sizeof(uint8_t));

  buffer[len] = value;
  len += sizeof(uint8_t);

  return buffer;
}

uint8_t *write16(uint8_t *buffer, uint16_t value, int len) {
  buffer = realloc(buffer, len + sizeof(uint16_t));
  value = jasLittleEndian16(value);

  buffer[len] = (value >> 8) & 0xFF;
  buffer[len + 1] = value & 0xFF;
  len += sizeof(uint16_t);

  return buffer;
}
