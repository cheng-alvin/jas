/* Extracted and modified from `README.md` */
#include <jas.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  instruction_t *instr = instr_gen(INSTR_AND, 2, r32(REG_EAX), imm8(0x12));
  buffer_t buf = assemble_instr(MODE_LONG, instr);

  for (int i = 0; i < buf.len; i++)
    printf("%02X ", buf.data[i]);

  free(buf.data);
  return 0;
}