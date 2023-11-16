#include "init.h"
#include "error.h"
#include "null.h"
#include <stdint.h>
#include <stdlib.h>

jasErrorCode_t jasInitNew(jasMode_t mode, jasInstance_t *instance, uint8_t *buffer) {
  // TODO Check if pointer `instance` is already allocated or not!

  instance->mode = mode;
  instance->buffer = buffer;

  return JAS_NO_ERROR;
}