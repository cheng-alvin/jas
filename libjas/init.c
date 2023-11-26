#include "init.h"
#include "error.h"
#include "null.h"
#include <stdint.h>
#include <stdlib.h>

jasErrorCode_t jasInitNew(jasMode_t mode, jasInstance_t *instance, uint8_t *buffer, jasOutFormat_t format, char **includes, char *filename, bool isFile) {
  if (instance == NULL)
    return JAS_ERROR_UNDEFINED_POINTER;

  instance->mode = mode;
  instance->buffer = buffer;
  instance->bufferLen = 0;
  instance->format = format == NULL ? JAS_OUT_FORMAT_RAW : format;
  instance->includes = includes;
  instance->filename = filename;
  instance->isFile = isFile;

  return JAS_NO_ERROR;
}