#include "null.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char **errors = NULL;
static int errorCount = 0;

void jasRegisterError(char *error) {
  errorCount++;
  errors = (char **)realloc(errors, sizeof(char *) * (errorCount));
  errors[errorCount - 1] = strdup(error);
}

char *jasPollLastError() {
  if (errorCount == 0) {
    return NULL;
  }

  char *lastError = errors[errorCount - 1];
  errors = (char **)realloc(errors, sizeof(char *) * (errorCount - 1));
  errorCount--;
  return lastError;
}

void jasPrintErrors() {
  while (jasPollLastError() != NULL) {
    fprintf(stderr, "%s\n", jasPollLastError());
  }
}