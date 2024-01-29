#include "error.h"

char *jasErrorno(jasErrorCode_t c) {
  char *codes[] = {"JAS_NO_ERROR", "JAS_UNDEFINED_POINTER", "JAS_INVALID_INSTRUCTION", "JAS_NON_LONG_MODE_INSTRUCTION", "JAS_OPERAND_ERROR", "JAS_MODE_ERROR", "JAS_INVALID_HIGH_REGISTER", "JAS_LONG_MODE_INSTRUCTION", "JAS_INTERNAL_ERROR", "JAS_RESERVED_ERROR"}; // TODO Add more error codes

  if (c > sizeof(codes) / sizeof(char *))
    return "JAS_NO_ERROR_CODE";

  return codes[c];
}
