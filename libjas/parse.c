
/**
 * MIT License
 * Copyright (c) 2023-2026 Alvin Cheng <eventide1029@gmail.com>
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
 * @see `LICENSE`
 */

#include "parse.h"
#include "error.h"
#include "register.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>

enum instructions parse_instr_name(char *name) {
  for (int i = 0; name[i]; i++)
    name[i] = (unsigned char)tolower((int)name[i]);

  // clang-format off
  /// @note `instr_tab_names` is defined in `instructions.inc`
  /// depicting instruction names in order of enum.

  /// The definitionof the the variable `INSTR_ENUM` is used to
  /// conditionally include the variable definition only here,
  /// in lieu of the entire instruction encoder reference table
  /// with substantial size.

  #define INSTR_ENUM
  #include "instructions.inc"
  // clang-format on

  for (int i = 0; i < sizeof(instr_tab_names); i++) {
    if (instr_tab_names[i][0] != name[0]) continue;
    if (strcmp(instr_tab_names[i], name) == 0)
      return (enum instructions)i + 1;
  }

  return INSTR_NULL;
}

uint64_t parse_str_num(char *name) {
  const uint8_t len = strlen(name);
  int base = 10;

  if (name[len] == 'h') base = 16;
  if (name[0] == '0' && name[1] == 'x') {
    base = 16;
    name += 2;
  }
  
  return strtoull(name, NULL, base);
}

// clang-format off

/// @brief  A list of all instruction names in order of the enum.
/// @see `registers` in `register.h` (Which this is in order of)

const char *reg_names[] = {
    "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh",
    "ax", "cx", "dx", "bx", "sp", "bp", "si", "di",
    "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi",
    "rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi",
    "r8b", "r9b", "r10b", "r11b", "r12b", "r13b", "r14b", "r15b",
    "r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w",
    "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d",
    "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15",
    "spl", "bpl", "sil", "dil", "ip", "eip", "rip",
};

// clang-format on

enum registers parse_reg(char *name) {
  for (int i = 0; name[i]; i++)
    name[i] = (unsigned char)tolower((int)name[i]);

  for (int i = 0; i < sizeof(reg_names); i++)
    if (strcmp(reg_names[i], name) == 0) return (enum registers)i;

  return NULL;
}