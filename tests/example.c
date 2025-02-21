/* Extracted and modified from `README.md` */
#include <jas.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void error_handler(const char *msg) {
  fprintf(stderr, "Error: %s\n", msg);
  exit(1);
}

int main(void) {
  err_add_callback(error_handler);
  instruction_t *instr[4];

  instr[2] = label_gen("label", LABEL_LOCAL);
  instr[1] = instr_gen(INSTR_MOV, 2, r64(REG_RAX), imm64(0));
  instr[0] = instr_gen(INSTR_JMP, 1, rel32("label", 0));

  buffer_t buf = codegen(MODE_LONG, &instr, 3, CODEGEN_RAW);

  for (int i = 0; i < buf.len; i++)
    printf("%02X ", buf.data[i]);

  free(buf.data);
  return 0;
}