/**
 * MIT License
 * Copyright (c) 2023-2024 Alvin Cheng <eventide1029@gmail.com>
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

#include "operand.h"
#include "register.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void test_printf(char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}

#define assert_not_null(x)                               \
  if ((x) == NULL) {                                     \
    test_printf("\nAssertion failed: %s is NULL\n", #x); \
    exit(1);                                             \
  }

#define RunTest(name, func) \
  test_##name##_##func();   \
  test_printf("[ \033[32mOK\033[0m ] %s::%s\n", #name, #func);

#define assert(x)                                            \
  if (!(x)) {                                                \
    test_printf("\nAssertion failed: %s is not true\n", #x); \
    exit(1);                                                 \
  }

#define assert_eq(x, y)                                    \
  if ((x) != (y)) {                                        \
    test_printf("\nAssertion failed: %s != %s\n", #x, #y); \
    exit(1);                                               \
  }

#define assert_str_eq(x, y, message) \
  if (strcmp((x), (y)) != 0) {       \
    test_printf("\n%s", message);    \
    exit(1);                         \
  }

#define Test(name, func) \
  void test_##name##_##func()

#define fail(message)                          \
  test_printf("\nTest failed: %s\n", message); \
  exit(1);

#define TestSuite(name)

static operand_t r8 = (operand_t){.type = OP_R8, .offset = 0, .data = &(enum registers){REG_AL}};
static operand_t r16 = (operand_t){.type = OP_R16, .offset = 0, .data = &(enum registers){REG_AX}};
static operand_t r32 = (operand_t){.type = OP_R32, .offset = 0, .data = &(enum registers){REG_EAX}};
static operand_t r64 = (operand_t){.type = OP_R64, .offset = 0, .data = &(enum registers){REG_RAX}};

static operand_t m8 = (operand_t){.type = OP_M8, .offset = 0xff, .data = &(enum registers){REG_AL}};
static operand_t m16 = (operand_t){.type = OP_M16, .offset = 0xff, .data = &(enum registers){REG_AX}};
static operand_t m32 = (operand_t){.type = OP_M32, .offset = 0xff, .data = &(enum registers){REG_EAX}};
static operand_t m64 = (operand_t){.type = OP_M64, .offset = 0xff, .data = &(enum registers){REG_RAX}};

static operand_t imm8 = (operand_t){.type = OP_IMM8, .offset = 0, .data = &(unsigned char){0xFF}};
static operand_t imm16 = (operand_t){.type = OP_IMM16, .offset = 0, .data = &(unsigned short){0xFFFF}};
static operand_t imm32 = (operand_t){.type = OP_IMM32, .offset = 0, .data = &(unsigned int){0xFFFFFFFF}};
static operand_t imm64 = (operand_t){.type = OP_IMM64, .offset = 0, .data = &(unsigned long){0xFFFFFFFFFFFFFFFF}};

static void test_error_handler(const char *msg) {
  fail(msg);
}

#define assert_eq_buf(a, b) assert_eq_buf_arr(a, b.data, b.len)
#define assert_eq_buf_arr(a, b, arr_len)                                                       \
  \ 
  if (a.len != arr_len) test_printf("\nAssertion failed: %s is not the same as %s\n", #a, #b); \
  for (size_t i = 0; i < a.len; i++) {                                                         \
    if (a.data[i] != b[i]) {                                                                   \
      test_printf("\nAssertion failed: `%s` is not the same as `%s`\n", #a, #b);               \
      exit(1);                                                                                 \
    }                                                                                          \
  }

#endif
