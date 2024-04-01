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

#ifndef REGISTER_H
#define REGISTER_H

// #define REG_AL 0
// #define REG_CL 1
// #define REG_DL 2
// #define REG_BL 3
// #define REG_AH 4
// #define REG_CH 5
// #define REG_DH 6
// #define REG_BH 7

// #define REG_AX 0
// #define REG_CX 1
// #define REG_DX 2
// #define REG_BX 3
// #define REG_SP 4
// #define REG_BP 5
// #define REG_SI 6
// #define REG_DI 7

// #define REG_EAX 0
// #define REG_ECX 1
// #define REG_EDX 2
// #define REG_EBX 3
// #define REG_ESP 4
// #define REG_EBP 5
// #define REG_ESI 6
// #define REG_EDI 7

// #define REG_RAX 0
// #define REG_RCX 1
// #define REG_RDX 2
// #define REG_RBX 3
// #define REG_RSP 4
// #define REG_RBP 5
// #define REG_RSI 6
// #define REG_RDI 7

// // REX.B dictated registers:

// #define REG_R8B 0
// #define REG_R9B 1
// #define REG_R10B 2
// #define REG_R11B 3
// #define REG_R12B 4
// #define REG_R13B 5
// #define REG_R14B 6
// #define REG_R15B 7

// #define REG_R8W 0
// #define REG_R9W 1
// #define REG_R10W 2
// #define REG_R11W 3
// #define REG_R12W 4
// #define REG_R13W 5
// #define REG_R14W 6
// #define REG_R15W 7

// #define REG_R8D 0
// #define REG_R9D 1
// #define REG_R10D 2
// #define REG_R11D 3
// #define REG_R12D 4
// #define REG_R13D 5
// #define REG_R14D 6
// #define REG_R15D 7

// #define REG_R8 0
// #define REG_R9 1
// #define REG_R10 2
// #define REG_R11 3
// #define REG_R12 4
// #define REG_R13 5
// #define REG_R14 6
// #define REG_R15 7

// #define REG_SPL 4
// #define REG_BPL 5
// #define REG_SIL 6
// #define REG_DIL 7

// // Segment registers: (But not dictated by REX.B)
// #define REG_ES 0
// #define REG_CS 1
// #define REG_SS 2
// #define REG_DS 3
// #define REG_FS 4
// #define REG_GS 5

enum registers {
  REG_AL,
  REG_CL,
  REG_DL,
  REG_BL,
  REG_AH,
  REG_CH,
  REG_DH,
  REG_BH,

  REG_AX,
  REG_CX,
  REG_DX,
  REG_BX,
  REG_SP,
  REG_BP,
  REG_SI,
  REG_DI,

  REG_EAX,
  REG_ECX,
  REG_EDX,
  REG_EBX,
  REG_ESP,
  REG_EBP,
  REG_ESI,
  REG_EDI,

  REG_RAX,
  REG_RCX,
  REG_RDX,
  REG_RBX,
  REG_RSP,
  REG_RBP,
  REG_RSI,
  REG_RDI,

  REG_R8B,
  REG_R9B,
  REG_R10,
  REG_R11,
  REG_R12,
  REG_R13,
  REG_R14,
  REG_R15,

  REG_R8W,
  REG_R9W,
  REG_R10,
  REG_R11,
  REG_R12,
  REG_R13,
  REG_R14,
  REG_R15,

  REG_R8D,
  REG_R9D,
  REG_R10,
  REG_R11,
  REG_R12,
  REG_R13,
  REG_R14,
  REG_R15,

  REG_R8,
  REG_R9,
  REG_R10,
  REG_R11,
  REG_R12,
  REG_R13,
  REG_R14,
  REG_R15,

  REG_SPL,
  REG_BPL,
  REG_SIL,
  REG_DIL,

  REG_ES,
  REG_CS,
  REG_SS,
  REG_DS,
  REG_FS,
  REG_GS,
};

// TODO(Also random side note I've realized) We need a way to ensure that the REX prefixes and registers work as intended with the B prefix and that high order registers do not conflict with REX prefixes!!

#endif
