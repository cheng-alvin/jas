#include <stdint.h>

typedef union {
  uint8_t u8;
  uint16_t u16;
  uint32_t u32;
  uint64_t u64;
} jasOperand_t;
