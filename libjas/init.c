#include "init.h"
#include "error.h"
#include "null.h"
#include <stdint.h>
#include <stdlib.h>

// TODO Add allocation for `buffer`
jasErrorCode_t jasInitNew(jasMode_t mode, jasInstance_t *instance, uint8_t *buffer, jasOutFormat_t format) {
  if (instance == JAS_NULL)
    return JAS_UNDEFINED_POINTER;

  instance->mode = mode;
  instance->buffer = buffer;
  instance->bufferLen = 0;
  instance->format = format == JAS_NULL ? JAS_OUT_FORMAT_RAW : format;
  return JAS_NO_ERROR;
}
