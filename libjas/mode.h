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

#ifndef MODE_H
#define MODE_H

#include <stdbool.h>
#include <stdint.h>

enum modes {
  MODE_REAL,
  MODE_PROTECTED,
  MODE_LONG,
};

/**
 * Type wrapper for an unsigned char that represents the
 * support status of an instruction in different intel x86
 * operation modes, like 64, 32 and 16 bit modes. The individual
 * bits of the byte represent the support status of the instruction
 * in the different operation modes. The bits are ordered from
 * least significant to most significant as follows:
 *
 * 0: 16 bit mode encoding support status
 * 1: 32 bit mode encoding support status
 * 2: 64 bit mode encoding support status
 *
 * 3-7: Reserved for future use
 *
 * A bit set to 1 indicates that the instruction is supported
 * in the corresponding operation mode, while a bit set to 0
 * indicates that the instruction is not supported in the
 * corresponding operation mode.
 *
 * 🤠
 *
 * There are some macros defined below to help you as well!
 */
typedef uint8_t mode_support_t;

/**
 * Macros for defining the different support status of an
 * instruction in the different intel x86 operation modes.
 *
 * You can just simply `|` them together to combine it!
 */

#define MODE_SUPPORT_16BIT 0b00000001
#define MODE_SUPPORT_32BIT 0b00000010
#define MODE_SUPPORT_64BIT 0b00000100
#define MODE_SUPPORT_ALL 0b00000111

/**
 * Function for checking if an instruction is allowed in a given mode
 * by checking if a bit is set in the support field.
 *
 * @param mode The mode to check.
 * @param support The support field of the instruction.
 * @return True if the mode is matched, otherwise false.
 *
 * @see mode_support_t
 */
bool mode_valid(const enum modes mode, const mode_support_t support);

/**
 * Wrapper macro in support of the `mode_valid` function.
 * As suggested in the `mode_valid()`, the function does not
 * check for the validity if the fallback flag is set in favour
 * for flexibility.
 *
 * @note This macro is tailored for internal usage ONLY
 * otherwise some variables and parts may be undefined.
 *
 * @see `instruction.h`
 */
#define check_mode(x, y)            \
  if (!mode_valid(x, y)) {          \
    err("Invalid operating mode."); \
    return;                         \
  }

#endif
