#ifndef TEST_H
#define TEST_H

void test(char *name, testFunction_t unit);
int getStatus(testFunction_t unit);
void terminateTest();

typedef void (*testFunction_t)(void);

extern static testInfo_t *info;

#endif