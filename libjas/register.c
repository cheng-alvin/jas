/**
 * MIT License
 * Copyright (c) 2023 Alvin Cheng (eventide1029@gmail.com)
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
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define REG_VALUE_AL 0
#define REG_VALUE_CL 1
#define REG_VALUE_DL 2
#define REG_VALUE_BL 3
#define REG_VALUE_AH 4
#define REG_VALUE_CH 5
#define REG_VALUE_DH 6
#define REG_VALUE_BH 7

#define REG_VALUE_AX 0
#define REG_VALUE_CX 1
#define REG_VALUE_DX 2
#define REG_VALUE_BX 3
#define REG_VALUE_SP 4
#define REG_VALUE_BP 5
#define REG_VALUE_SI 6
#define REG_VALUE_DI 7

#define REG_VALUE_EAX 0
#define REG_VALUE_ECX 1
#define REG_VALUE_EDX 2
#define REG_VALUE_EBX 3
#define REG_VALUE_ESP 4
#define REG_VALUE_EBP 5
#define REG_VALUE_ESI 6
#define REG_VALUE_EDI 7

#define REG_VALUE_RAX 0
#define REG_VALUE_RCX 1
#define REG_VALUE_RDX 2
#define REG_VALUE_RBX 3
#define REG_VALUE_RSP 4
#define REG_VALUE_RBP 5
#define REG_VALUE_RSI 6
#define REG_VALUE_RDI 7

// REX.B dictated registers:

#define REG_VALUE_R8B 0
#define REG_VALUE_R9B 1
#define REG_VALUE_R10B 2
#define REG_VALUE_R11B 3
#define REG_VALUE_R12B 4
#define REG_VALUE_R13B 5
#define REG_VALUE_R14B 6
#define REG_VALUE_R15B 7

#define REG_VALUE_R8W 0
#define REG_VALUE_R9W 1
#define REG_VALUE_R10W 2
#define REG_VALUE_R11W 3
#define REG_VALUE_R12W 4
#define REG_VALUE_R13W 5
#define REG_VALUE_R14W 6
#define REG_VALUE_R15W 7

#define REG_VALUE_R8D 0
#define REG_VALUE_R9D 1
#define REG_VALUE_R10D 2
#define REG_VALUE_R11D 3
#define REG_VALUE_R12D 4
#define REG_VALUE_R13D 5
#define REG_VALUE_R14D 6
#define REG_VALUE_R15D 7

#define REG_VALUE_R8 0
#define REG_VALUE_R9 1
#define REG_VALUE_R10 2
#define REG_VALUE_R11 3
#define REG_VALUE_R12 4
#define REG_VALUE_R13 5
#define REG_VALUE_R14 6
#define REG_VALUE_R15 7

#define REG_VALUE_SPL 4
#define REG_VALUE_BPL 5
#define REG_VALUE_SIL 6
#define REG_VALUE_DIL 7

// Segment registers: (But not dictated by REX.B)
#define REG_VALUE_ES 0
#define REG_VALUE_CS 1
#define REG_VALUE_SS 2
#define REG_VALUE_DS 3
#define REG_VALUE_FS 4
#define REG_VALUE_GS 5

uint8_t reg_lookup_val(enum registers input) {
  uint8_t lookup[] = {REG_VALUE_AL, REG_VALUE_CL, REG_VALUE_DL, REG_VALUE_BL, REG_VALUE_AH, REG_VALUE_CH, REG_VALUE_DH, REG_VALUE_BH, REG_VALUE_AX, REG_VALUE_CX, REG_VALUE_DX, REG_VALUE_BX, REG_VALUE_SP, REG_VALUE_BP, REG_VALUE_SI, REG_VALUE_DI, REG_VALUE_EAX, REG_VALUE_ECX, REG_VALUE_EDX, REG_VALUE_EBX, REG_VALUE_ESP, REG_VALUE_EBP, REG_VALUE_ESI, REG_VALUE_EDI, REG_VALUE_RAX, REG_VALUE_RCX, REG_VALUE_RDX, REG_VALUE_RBX, REG_VALUE_RSP, REG_VALUE_RBP, REG_VALUE_RSI, REG_VALUE_RDI, REG_VALUE_R8B, REG_VALUE_R9B, REG_VALUE_R10B, REG_VALUE_R11B, REG_VALUE_R12B, REG_VALUE_R13B, REG_VALUE_R14B, REG_VALUE_R15B, REG_VALUE_R8W, REG_VALUE_R9W, REG_VALUE_R10W, REG_VALUE_R11W, REG_VALUE_R12W, REG_VALUE_R13W, REG_VALUE_R14W, REG_VALUE_R15W, REG_VALUE_R8D, REG_VALUE_R9D, REG_VALUE_R10D, REG_VALUE_R11D, REG_VALUE_R12D, REG_VALUE_R13D, REG_VALUE_R14D, REG_VALUE_R15D, REG_VALUE_R8, REG_VALUE_R9, REG_VALUE_R10, REG_VALUE_R11, REG_VALUE_R12, REG_VALUE_R13, REG_VALUE_R14, REG_VALUE_R15, REG_VALUE_SPL, REG_VALUE_BPL, REG_VALUE_SIL, REG_VALUE_DIL, REG_VALUE_ES, REG_VALUE_CS, REG_VALUE_SS, REG_VALUE_DS, REG_VALUE_FS, REG_VALUE_GS};

  return lookup[(size_t)input];
}