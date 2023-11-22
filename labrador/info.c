#include "info.h"
#include "color.h"
#include "test.h"
#include <stdio.h>
#include <stdlib.h>

testInfo_t *info;

int getStatus(testFunction_t unit) {
  const size_t infoLen = sizeof(*info) / sizeof(testInfo_t);
  const int i = getIndexOfEntryWithFunction(unit);
  info = realloc(info, infoLen - sizeof(testInfo_t));
  return info[i].status;
}

void addStatus(testFunction_t unit, char *name) {
  const size_t infoLen = sizeof(*info) / sizeof(testInfo_t);
  info = realloc(info, infoLen + sizeof(testInfo_t));
  info[infoLen].function = unit;
  info[infoLen].name = name;
  info[infoLen].status = 0;
}

int getIndexOfEntry(char *name) {
  const size_t infoLen = sizeof(*info) / sizeof(testInfo_t);

  for (int i = 0; (size_t)i < infoLen; i++) {
    if (info[i].name == name) {
      return i;
    }
  }

  printf(BOLD RED "failure:%s Unable to locate unit test when getting status (Reading %s) \n%s", BOLD, name, RESET);
  exit(1);
}

int getIndexOfEntryWithFunction(testFunction_t unit) {
  const size_t infoLen = sizeof(*info) / sizeof(testInfo_t);

  for (int i = 0; (size_t)i < infoLen; i++) {
    if (info[i].function == unit) {
      return i;
    }
  }

  printf(BOLD RED "failure:%s Unable to locate unit test when getting status (Reading %p) \n%s", BOLD, (void *)unit, RESET);
  exit(1);
}

void fail(testFunction_t unit) {
  info[getIndexOfEntryWithFunction(unit)].status = 1;
}