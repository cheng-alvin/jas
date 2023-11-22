#include "color.h"
#include <stdio.h>

void test(char *name, int (*unit)(char *name)) {
  const int retValue = unit(name);
  if (retValue != 0) {
    printf(BOLD RED "failure:%s %s\n%s", BOLD, name, RESET);
    return;
  }

  printf(BOLD GREEN "success%s %s\n%s", BOLD, name, RESET);
}