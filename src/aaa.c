#include "error.h"
#include "hint.h"
#include "null.h"
#include "write.h"
#include <stdint.h>

uint8_t *aaa() {
  if (jasGetMode() == JAS_LONG_MODE) {
    jasRegisterError("`aaa` instruction not supported in long-mode\n");
    return NULL;
  }

  return write8(jasGetBuffer(), 0x37, sizeof(*(jasGetBuffer())) / sizeof(uint8_t));
}