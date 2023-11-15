#include "hint.h"
#include "error.h"
#include <stdint.h>

static uint8_t *buffer;
static jasMode_t mode;

void jasHint(hint_t type, const long data) {
  switch (type) {
  case JAS_BUFFER:
    if (sizeof(data) != sizeof(uint8_t *)) {
      jasRegisterError("Invalid data size for hinting information bound for: `JAS_BUFFER`");
      break;
    }

    buffer = (uint8_t *)data;
    break;

  case JAS_OPERATION_MODE:
    if (sizeof(data) != sizeof(jasMode_t)) {
      jasRegisterError("Invalid data size for hinting information bound for: `JAS_OPERATION_MODE`");
      break;
    }

    mode = (jasMode_t)data;
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