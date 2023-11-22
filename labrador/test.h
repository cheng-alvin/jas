#ifndef TEST_H
#define TEST_H

typedef void (*testFunction_t)(void);
typedef struct {
  testFunction_t function;
  int status;
  char *name;
} testInfo_t;

void test(char *name, testFunction_t unit);
int getStatus(testFunction_t unit);
void terminateTest();

extern testInfo_t *info;

#endif