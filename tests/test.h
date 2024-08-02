/**
 * MIT License
 * Copyright (c) 2023-2024 Alvin Cheng (eventide1029@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * ---
 *
 * Simple and dumb testing framework used internally for
 * testing the assembler's instruction encoding and other
 * chores. Feel free to copy and modify this for your own
 * testing and programming purposes. :)
 */

#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void test_printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}

#define RunTest(name, func)          \
  test_printf("Running tests...\n"); \
  test_##name##_##func();            \
  test_printf("Test %s::%s passed.\n", #name, #func);

#define assert(x)                                          \
  if (!(x)) {                                              \
    test_printf("Assertion failed: %s is not true\n", #x); \
    exit(1);                                               \
  }

#define assert_eq(x, y)                                  \
  if ((x) != (y)) {                                      \
    test_printf("Assertion failed: %s != %s\n", #x, #y); \
    exit(1);                                             \
  }

#define assert_str_eq(x, y, message) \
  if (strcmp((x), (y)) != 0) {       \
    test_printf("%s", ##message);    \
    exit(1);                         \
  }

#define Test(name, func) \
  void test_##name##_##func()

#define fail(message)                        \
  test_printf("Test failed: %s\n", message); \
  exit(1);

#define TestSuite(name) \
  test_printf("Running test suite %s\n", #name);
#endif