#include "test.h"
#include "color.h"
#include "info.h"
#include <stdio.h>
#include <stdlib.h>

static int exitVal;

void test(char *name, testFunction_t unit) {
  addStatus(unit, name);
  unit();
  const int retValue = getStatus(unit);

  if (retValue != 0) {
    printf(BOLD RED "failure:%s %s\n%s", BOLD, name, RESET);
    exitVal++;
    return;
  }

  printf(BOLD GREEN "success%s %s\n%s", BOLD, name, RESET);
}

void terminateTest() {
  free(info);
  exit(exitVal);
}
