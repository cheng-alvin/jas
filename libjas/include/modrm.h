#include <stdint.h>
// TODO Add docs:
// TODO Header guards?

typedef enum {
  JAS_MODRM_REGISTER_OPERAND = 0b00000000,
  JAS_MODRM_ADDRESS = 0b01000000,
  JAS_MODRM_32_DISPLACEMENT = 0b10000000,
  JAS_MODRM_REGISTER = 0b11000000,
} jasModRMMode_t;

// TODO Docs:

uint8_t jasGenerateModRM(jasModRMMode_t mod, uint8_t reg, uint8_t rm);
