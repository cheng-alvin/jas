#include "endian.h"
#include "parse.h"
#include "write.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  uint8_t *buf = (uint8_t *)malloc(1);
  int len = 0;
  JAS_write_8(buf, 0xFF, &len);
  JAS_write_16(buf, 0xAD98, &len);
  JAS_write_16(buf, 0x1235, &len);

  for (int i = 0; i < 10; i++) {
    printf("%02X\n ", buf[i]);
  }

  return 0;
}