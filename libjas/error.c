#include "error.h"

char *jasErrorno(jasErrorCode_t c) {
  char *codes[] = {"JAS_NO_ERROR", "JAS_ERROR_UNDEFINED_POINTER"}; // TODO Add more error codes here!
  return codes[c];
}
