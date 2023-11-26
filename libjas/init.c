#include "init.h"
#include "error.h"
#include "null.h"
#include <stdint.h>
#include <stdlib.h>

jasErrorCode_t jasInitNew(jasMode_t mode, jasInstance_t *instance, uint8_t *buffer) {
  instance->mode = mode;
  instance->buffer = buffer;
  instance->bufferLen = 0;

  return JAS_NO_ERROR;
}