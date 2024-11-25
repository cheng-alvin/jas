#include "jas.h"
#include "test.h"

void error_callback(const char *msg) {
  printf("%s\n", msg);
  exit(1);
}

Test(example, example) {
  err_add_callback(error_callback);

  instruction_t instr[] = (instruction_t[]){
      // mov double(int), rax
      (instruction_t){
          .instr = INSTR_MOV,
          .operands = (operand_t[]){
              (operand_t){
                  .type = OP_M64,
                  .data = &(enum registers){REG_RBP},
                  .offset = 0,
                  //   .label = "double(int)",
              },
              (operand_t){
                  .type = OP_R64,
                  .data = &(enum registers){REG_RAX},
              },
              OP_NONE,
              OP_NONE,
          },
      },
      // square(int):
      (instruction_t){
          .instr = INSTR_DIR_GLOBAL_LABEL,
          .operands = (operand_t[]){
              (operand_t){.type = OP_MISC, .data = "double(int)"},
              OP_NONE,
              OP_NONE,
              OP_NONE,
          },
      },

  };

  buffer_t buf = codegen(MODE_LONG, instr, sizeof(instr), CODEGEN_ELF);

  for (size_t i = 0; i < buf.len; i++) {
    printf("%02X ", buf.data[i]);
  }

  FILE *file = fopen("main.o", "wb");
  size_t written = fwrite(buf.data, sizeof(uint8_t), buf.len, file);
  fclose(file);

  free(buf.data);
}

int main(void) {
  TestSuite(example);
  RunTest(example, example);

  return 0;
}
