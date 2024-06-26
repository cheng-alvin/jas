#include <stddef.h>
#include <stdint.h>

void endian(uint8_t *data, size_t data_size) {
  uint8_t result[data_size];
  for (size_t i = data_size; i > 0; i--)
    result[data_size - i] = data[i];

  data = result;
}
