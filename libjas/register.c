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
 * @see `LICENSE`
 */

#include "register.h"
#include <stdbool.h>
#include <stdint.h>

uint8_t reg_lookup_val(enum registers *input) {
  uint8_t lookup[] = {
      REG_VALUE_AL, REG_VALUE_CL, REG_VALUE_DL, REG_VALUE_BL, REG_VALUE_AH,
      REG_VALUE_CH, REG_VALUE_DH, REG_VALUE_BH, REG_VALUE_AX, REG_VALUE_CX, REG_VALUE_DX,
      REG_VALUE_BX, REG_VALUE_SP, REG_VALUE_BP, REG_VALUE_SI, REG_VALUE_DI, REG_VALUE_EAX,
      REG_VALUE_ECX, REG_VALUE_EDX, REG_VALUE_EBX, REG_VALUE_ESP, REG_VALUE_EBP, REG_VALUE_ESI,
      REG_VALUE_EDI, REG_VALUE_RAX, REG_VALUE_RCX, REG_VALUE_RDX, REG_VALUE_RBX, REG_VALUE_RSP,
      REG_VALUE_RBP, REG_VALUE_RSI, REG_VALUE_RDI, REG_VALUE_R8B, REG_VALUE_R9B, REG_VALUE_R10B,
      REG_VALUE_R11B, REG_VALUE_R12B, REG_VALUE_R13B, REG_VALUE_R14B, REG_VALUE_R15B, REG_VALUE_R8W,
      REG_VALUE_R9W, REG_VALUE_R10W, REG_VALUE_R11W, REG_VALUE_R12W, REG_VALUE_R13W, REG_VALUE_R14W,
      REG_VALUE_R15W, REG_VALUE_R8D, REG_VALUE_R9D, REG_VALUE_R10D, REG_VALUE_R11D, REG_VALUE_R12D,
      REG_VALUE_R13D, REG_VALUE_R14D, REG_VALUE_R15D, REG_VALUE_R8, REG_VALUE_R9, REG_VALUE_R10,
      REG_VALUE_R11, REG_VALUE_R12, REG_VALUE_R13, REG_VALUE_R14, REG_VALUE_R15, REG_VALUE_SPL,
      REG_VALUE_BPL, REG_VALUE_SIL, REG_VALUE_DIL, REG_VALUE_ES, REG_VALUE_CS, REG_VALUE_SS,
      REG_VALUE_DS, REG_VALUE_FS, REG_VALUE_GS};

  return lookup[(unsigned int)*input];
}

bool reg_needs_rex(enum registers input) {
  if (input > REG_RDI && input < REG_ES)
    return true;

  return false;
}
