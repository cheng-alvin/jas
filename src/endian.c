#include <stdint.h>

uint16_t littleEndian16(uint16_t value) {
  return ((value & 0xFFU) << 8) | ((value & 0xFF00U) >> 8);
}

uint32_t littleEndian32(uint32_t value) {
  return ((value & 0xFF) << 24) |
         ((value & 0xFF00) << 8) |
         ((value & 0xFF0000) >> 8) |
         ((value & 0xFF000000) >> 24);
}

uint64_t littleEndian64(uint64_t value) {
  return ((value & 0xFFULL) << 56) |
         ((value & 0xFF00ULL) << 40) |
         ((value & 0xFF0000ULL) << 24) |
         ((value & 0xFF000000ULL) << 8) |
         ((value & 0xFF00000000ULL) >> 8) |
         ((value & 0xFF0000000000ULL) >> 24) |
         ((value & 0xFF000000000000ULL) >> 40) |
         ((value & 0xFF00000000000000ULL) >> 56);
}