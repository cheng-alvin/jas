#include "size.h"
#include "error.h"
#include "null.h"
#include "operand.h"
#include <stdint.h>

int8_t jasOperandSize(jasOperand_t x) {
  if (x.operand8 != NULL)
    return 8;
  if (x.operand16 != NULL)
    return 16;
  if (x.operand32 != NULL)
    return 32;
  if (x.operand64 != NULL)
    return 64;

  if (x.reg.reg8 != NULL || x.reg.reg16 != NULL || x.reg.reg32 != NULL || x.reg.reg64 != NULL)
    return -1;

  return 0;
}

int8_t jasRegisterSize(jasOperand_t x) {
  if (x.reg.reg8 != NULL)
    return 8;
  if (x.reg.reg16 != NULL)
    return 16;
  if (x.reg.reg32 != NULL)
    return 32;
  if (x.reg.reg64 != NULL)
    return 64;

  if (x.operand8 != NULL || x.operand16 != NULL || x.operand32 != NULL || x.operand64 != NULL)
    return -1;

  return 0;
}