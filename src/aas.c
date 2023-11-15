#include "error.h"
#include "hint.h"
#include "null.h"
#include "write.h"
#include <stdint.h>

uint8_t *aas() {
  if (jasGetMode() == JAS_LONG_MODE) {
    jasRegisterError("`aas` instruction not supported in long-mode\n");
    return NULL;
  }

  return write8(jasGetBuffer(), 0x3F, sizeof(*(jasGetBuffer())) / sizeof(uint8_t));
}