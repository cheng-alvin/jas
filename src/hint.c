#include "hint.h"
#include <stdint.h>

static uint8_t *buffer;
static mode_t mode;

void jasHint(hint_t type, const void *data) {
  switch (type) {
  case JAS_BUFFER:
    buffer = (uint8_t *)data;
    break;

  case JAS_OPERATION_MODE:
    mode = (mode_t)data;
    break;

  default:
    break;
  }
}

uint8_t *jasGetBuffer() {
  return buffer;
}

uint8_t jasGetMode() {
  return mode;
}