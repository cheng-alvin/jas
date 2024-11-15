#include "jas.h"
#include "test.h"

void error_callback(const char *msg) {
  printf("%s\n", msg);
  exit(1);
}

Test(example, example) {
  err_add_callback(error_callback);

  instruction_t instr[] = (instruction_t[]){
      // nop
      (instruction_t){
          .instr = INSTR_NOP,
          .operands = (operand_t[]){
              OP_NONE,
              OP_NONE,
              OP_NONE,
              OP_NONE,
          },
      },

      // mov rax, 60
      (instruction_t){
          .instr = INSTR_MOV,
          .operands = (operand_t[]){
              (operand_t){
                  .type = OP_R64,
                  .data = &(enum registers){REG_RAX},
              },
              (operand_t){
                  .type = OP_IMM64,
                  .data = &(uint64_t){60},
              },
              OP_NONE,
              OP_NONE,
          },
      },
      // xor rdi, rdi
      (instruction_t){
          .instr = INSTR_XOR,
          .operands = (operand_t[]){
              (operand_t){
                  .type = OP_R64,
                  .data = &(enum registers){REG_RDI},
              },
              (operand_t){
                  .type = OP_R64,
                  .data = &(enum registers){REG_RDI},
              },
              OP_NONE,
              OP_NONE,
          },
      },
      // global _start:
      (instruction_t){
          .instr = INSTR_DIR_EXTERN_LABEL,
          .operands = (operand_t[]){
              (operand_t){.type = OP_MISC, .data = "_start"},
              OP_NONE,
              OP_NONE,
              OP_NONE,
          },
      },
      // syscall
      (instruction_t){
          .instr = INSTR_SYSCALL,
          .operands = (operand_t[]){
              OP_NONE,
              OP_NONE,
              OP_NONE,
              OP_NONE,
          },
      },

      // jmp _start
      (instruction_t){
          .instr = INSTR_JMP,
          .operands = (operand_t[]){
              (operand_t){
                  .type = OP_REL32,
                  .label = "_start",
              },
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
