#include "color.h"
#include "null.h"
#include <stdio.h>

/**
 * @author cheng-alvin
 *
 * This is the labrador testing framework built for the Jas
 * assembler project. The testing framwork allows code to be tested
 * and have the verbose overloading be done by the preprocessor.
 *
 * The testing framework is used to test components of the Jas
 * assembler project and can be ported to other projects as well,
 * noting the perquisites needed to do so.
 *
 * @note null.h and color.h are required for the testing framework,
 * they can all be found inside the `util`'s include directory.
 */

#define TEST(name)   \
  \ 
    void __##name(); \
  __##name();        \
  \ 
    void __##name()

#define ASSERT(expr)                                                                                  \
  if (!(expr)) {                                                                                      \
    printf(RED BOLD "failure: " RESET BOLD "`" #expr "` did not match with expected result\n" RESET); \
    exit(1)                                                                                           \
  }

#define SHOULD_EQUAL(a, b) ASSERT(a == b)
#define SHOULD_NOT_EQUAL(a, b) ASSERT(a != b)
#define SHOULD_BE_TRUE(a) ASSERT(a)
#define SHOULD_BE_FALSE(a) ASSERT(!a)
#define SHOULD_BE_NULL(a) ASSERT(a == NULL)

#define TEST_SUITE int main(void)