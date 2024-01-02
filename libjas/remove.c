#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

uint8_t *removeElement(uint8_t *buffer, size_t length, size_t index) {
  if (index < length) {
    for (size_t i = index; i < length - 1; i++) {
      buffer[i] = buffer[i + 1];
    }
    return realloc(buffer, (length - 1) * sizeof(uint8_t));
  }
}