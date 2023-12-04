#include "error.h"

char *jasErrorno(jasErrorCode_t c) {
  char *codes[] = {"JAS_NO_ERROR", "JAS_ERROR_UNDEFINED_POINTER", "JAS_ERROR_INVALID_INSTRUCTION", "JAS_ERROR_NON_LONG_MODE_INSTRUCTION", "JAS_OPERAND_ERROR"}; // TODO Add more error codes

  if (c > sizeof(c) * sizeof(char *))
    return "JAS_NO_ERROR_CODE";

  return codes[c];
}
