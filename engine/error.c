#include "error.h"

char *jasErrorno(jasErrorCode_t c) {
  char *codes[] = {"JAS_NO_ERROR"}; // TODO Add more error codes here!
  return codes[c];
}