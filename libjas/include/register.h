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

#ifndef REGISTER_H
#define REGISTER_H

#include <stdbool.h>
#include <stdint.h>

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
  REG_R10B,
  REG_R11B,
  REG_R12B,
  REG_R13B,
  REG_R14B,
  REG_R15B,

  REG_R8W,
  REG_R9W,
  REG_R10W,
  REG_R11W,
  REG_R12W,
  REG_R13W,
  REG_R14W,
  REG_R15W,

  REG_R8D,
  REG_R9D,
  REG_R10D,
  REG_R11D,
  REG_R12D,
  REG_R13D,
  REG_R14D,
  REG_R15D,

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

  REG_IP,
  REG_EIP,
  REG_RIP,
};

/**
 * Macro definition of the register values. These are the values that
 * are used in the instruction encoding, unlike the enum values which
 * serves as a 'marker', or a key value pair.
 *
 * @note Not to be confused with the enum `registers`
 */

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

// Misc and instruction pointer registers:
#define REG_VALUE_IP 5
#define REG_VALUE_EIP 5
#define REG_VALUE_RIP 5

/**
 * Lookup table wrapper for finding register values from a single
 * centralized enumeration value, as per above's enum `registers`.
 *
 * @param input enum registers' pointer value
 * @return uint8_t register value
 */
uint8_t reg_lookup_val(enum registers *input);

/**
 * Function for checking wether a register needs to have a REX.B
 * prefix before the opcode, the REX.B prefix is used to access
 * more general-purpose registers such as the higher 8 registers
 * in 64-bit mode.
 *
 * @param input enum registers value
 * @return Whether the register needs a REX.B prefix when encoded
 * @see `registers` and rex.h
 */
bool reg_needs_rex(enum registers input);

#endif
