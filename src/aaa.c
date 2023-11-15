#include "error.h"
#include "hint.h"
#include "null.h"
#include "write.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t *aaa(uint8_t *buffer) {
  if (jasGetMode() == JAS_LONG_MODE) {
    jasRegisterError("`aaa` instruction not supported in long-mode\n");
    return NULL;
  }

  return write8(buffer, 0x37, sizeof(*buffer) / sizeof(uint8_t));
}