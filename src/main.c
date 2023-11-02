#include "endian.h"
#include "instructions.h"
#include "parse.h"
#include "registers.h"
#include "write.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  uint8_t *buf = (uint8_t *)malloc(1);
  int len = 0;
  write8(buf, 0xFF, &len);
  write16(buf, 0xAD98, &len);
  write16(buf, 0x1235, &len);

  for (int i = 0; i < 10; i++) {
    printf("%02X\n ", buf[i]);
  }

  return 0;
}