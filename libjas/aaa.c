#include "codegen.h"
#include "error.h"
#include "init.h"
#include "write.h"
#include <stdint.h>

jasErrorCode_t aaa(jasOperand_t op1, jasOperand_t op2, jasOperand_t op3, jasOperand_t op4, uint8_t **buffer, jasInstance_t instance) {
  *buffer = write8(buffer, 0x37, sizeof(buffer) / sizeof(uint8_t));
  return JAS_NO_ERROR;
}