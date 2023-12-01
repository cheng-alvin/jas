#include <stdint.h>

// TODO Add documentation.

typedef union {
  uint8_t u8;
  uint16_t u16;
  uint32_t u32;
  uint64_t u64;
} jasOperand_t;

typedef jasErrorCode_t (*jasInstructionEncoderPointerArray_t[])(jasOperand_t, jasOperand_t, jasOperand_t, jasOperand_t, jasInstance_t);
