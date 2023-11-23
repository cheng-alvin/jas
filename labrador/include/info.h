#ifndef INFO_H
#define INFO_H

#include "test.h"

int getStatus(testFunction_t unit);
void addStatus(testFunction_t unit, char *name);
int getIndexOfEntry(char *name);
int getIndexOfEntryWithFunction(testFunction_t unit);

#endif