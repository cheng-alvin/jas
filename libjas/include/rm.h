#include "error.h"
#include "init.h"
#include "macro.h"
#include "modrm.h"
#include "operand.h"

jasErrorCode_t jasExtendedOperandIdentityRM(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance, uint8_t mode, uint8_t opcodeExtention);