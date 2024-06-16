#include "buffer.h"
#include "codegen.h"
#include "mode.h"
#include "operand.h"
#include "register.h"

int main(void) {
  operand_t op1 = {.type = OP_R64, .data = (void *)REG_RAX};
  operand_t op2 = {.type = OP_R64, .data = (void *)REG_RBX};

  instruction_t arr[] = {
      {.instr = INSTR_MOV,
       .operands = &op1},
      {.instr = INSTR_MOV,
       .operands = &op2}};

  buffer_t buf = codegen(MODE_LONG, arr, sizeof(arr));
  return 0;
}