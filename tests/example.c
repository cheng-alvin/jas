#include "codegen.h"
#include "error.h"
#include "instruction.h"
#include "test.h"

void error_callback(const char *msg) {
  printf("%s\n", msg);
  exit(1);
}

Test(example, example) {
  err_add_callback(error_callback);

  instruction_t instr[] = (instruction_t[]){
      // mov rdx, 0x6000da
      (instruction_t){
          .instr = INSTR_MOV,
          .operands = (operand_t[]){
              (operand_t){
                  .type = OP_R64,
                  .data = &(enum registers){REG_RDX},
              },
              (operand_t){
                  .type = OP_IMM64,
                  .data = &(uint64_t){0x00000000006000da},
              },
              OP_NONE,
              OP_NONE,
          },
      },

      // mov rbx, 0x01
      (instruction_t){
          .instr = INSTR_MOV,
          .operands = (operand_t[]){
              (operand_t){
                  .type = OP_R64,
                  .data = &(enum registers){REG_RBX},
              },
              (operand_t){
                  .type = OP_IMM64,
                  .data = &(uint64_t){0x01},
              },
              OP_NONE,
              OP_NONE,
          },
      },

      // mov eax, 1
      (instruction_t){
          .instr = INSTR_MOV,
          .operands = (operand_t[]){
              (operand_t){
                  .type = OP_R32,
                  .data = &(enum registers){REG_EAX},
              },
              (operand_t){
                  .type = OP_IMM32,
                  .data = &(uint32_t){0x01},
              },
              OP_NONE,
              OP_NONE,
          },
      },
  };

  buffer_t buf = codegen(MODE_LONG, instr, sizeof(instr));
  for (uint8_t i = 0; i < buf.len; i++) {
    printf("%02x ", buf.data[i]);
  }
}

int main(void) {
  TestSuite(example);
  RunTest(example, example);

  return 0;
}
