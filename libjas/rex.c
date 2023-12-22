#include "rex.h"
#include <stdint.h>

#define REX_BASE 0b01000000

uint8_t jasRexConstructPrefix(uint8_t existingByte, jasRexPrefix_t prefixType) {
  if (existingByte)
    return existingByte | (uint8_t)prefixType;

  return REX_BASE | (uint8_t)prefixType;
}