#include "jas.h"
#include "test.h"

void error_callback(const char *msg) {
  printf("%s\n", msg);
  exit(1);
}

Test(example, example) {
  err_add_callback(error_callback);

  instruction_t instr[] = (instruction_t[]){
      //   // jmp _start
      //   (instruction_t){
      //       .instr = INSTR_JMP,
      //       .operands = (operand_t[]){
      //           (operand_t){
      //               .type = OP_REL32,
      //               .label = "_start",
      //           },
      //           OP_NONE,
      //           OP_NONE,
      //           OP_NONE,
      //       },
      //   },
      //   // global _start:
      //   (instruction_t){
      //       .instr = INSTR_DIR_GLOBAL_LABEL,
      //       .operands = (operand_t[]){
      //           (operand_t){.type = OP_MISC, .data = "_start"},
      //           OP_NONE,
      //           OP_NONE,
      //           OP_NONE,
      //       },
      //   },

      //   // mov rax, 60
      //   (instruction_t){
      //       .instr = INSTR_MOV,
      //       .operands = (operand_t[]){
      //           (operand_t){
      //               .type = OP_R64,
      //               .data = &(enum registers){REG_RAX},
      //           },
      //           (operand_t){
      //               .type = OP_IMM64,
      //               .data = &(uint64_t){60},
      //           },
      //           OP_NONE,
      //           OP_NONE,
      //       },
      //   },
      //   // xor rdi, rdi
      //   (instruction_t){
      //       .instr = INSTR_XOR,
      //       .operands = (operand_t[]){
      //           (operand_t){
      //               .type = OP_R64,
      //               .data = &(enum registers){REG_RDI},
      //           },
      //           (operand_t){
      //               .type = OP_R64,
      //               .data = &(enum registers){REG_RDI},
      //           },
      //           OP_NONE,
      //           OP_NONE,
      //       },
      //   },
      //   // syscall
      //   (instruction_t){
      //       .instr = INSTR_SYSCALL,
      //       .operands = (operand_t[]){
      //           OP_NONE,
      //           OP_NONE,
      //           OP_NONE,
      //           OP_NONE,
      //       },
      //   },

      /**
       * square(int):
          push    rbp
          mov     rbp, rsp
          mov     DWORD PTR [rbp-4], edi
          mov     eax, DWORD PTR [rbp-4]
          imul    eax, eax
          pop     rbp
          ret
       */

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
      // push rbp
      (instruction_t){
          .instr = INSTR_PUSH,
          .operands = (operand_t[]){
              (operand_t){
                  .type = OP_R64,
                  .data = &(enum registers){REG_RBP},
              },
              OP_NONE,
              OP_NONE,
              OP_NONE,
          },
      },
      // mov rbp, rsp
      (instruction_t){
          .instr = INSTR_MOV,
          .operands = (operand_t[]){
              (operand_t){
                  .type = OP_R64,
                  .data = &(enum registers){REG_RBP},
              },
              (operand_t){
                  .type = OP_R64,
                  .data = &(enum registers){REG_RSP},
              },
              OP_NONE,
              OP_NONE,
          },
      },
      // mov DWORD PTR [rbp-4], edi
      (instruction_t){
          .instr = INSTR_MOV,
          .operands = (operand_t[]){
              (operand_t){
                  .type = OP_M32,
                  .offset = -4,
                  .data = &(enum registers){REG_EBP},
              },
              (operand_t){
                  .type = OP_R32,
                  .data = &(enum registers){REG_EDI},
              },
              OP_NONE,
              OP_NONE,
          },
      },

      // mov eax, DWORD PTR [rbp-4]
      (instruction_t){
          .instr = INSTR_MOV,
          .operands = (operand_t[]){
              (operand_t){
                  .type = OP_R32,
                  .data = &(enum registers){REG_EAX},
              },
              (operand_t){
                  .type = OP_M32,
                  .offset = -4,
                  .data = &(enum registers){REG_EBP},
              },
              OP_NONE,
              OP_NONE,
          },
      },
      // imul eax, eax
      (instruction_t){
          .instr = INSTR_ADD,
          .operands = (operand_t[]){
              (operand_t){
                  .type = OP_R32,
                  .data = &(enum registers){REG_EAX},
              },
              (operand_t){
                  .type = OP_R32,
                  .data = &(enum registers){REG_EAX},
              },
              OP_NONE,
              OP_NONE,
          },
      },
      // pop rbp
      (instruction_t){
          .instr = INSTR_POP,
          .operands = (operand_t[]){
              (operand_t){
                  .type = OP_R64,
                  .data = &(enum registers){REG_RBP},
              },
              OP_NONE,
              OP_NONE,
              OP_NONE,
          },
      },
      // ret
      (instruction_t){
          .instr = INSTR_RET,
          .operands = (operand_t[]){
              OP_NONE,
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
