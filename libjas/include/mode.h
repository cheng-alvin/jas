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

#include "instruction.h"
#include <stdbool.h>

/**
 * Enums representing the operating modes of
 * the x86 processor as defined by the Intel
 * manual.
 */
enum modes {
  MODE_REAL,
  MODE_PROTECTED,
  MODE_LONG,
};

/**
 * Function for checking if an instruction is allowed in a given mode
 * by checking if a bit is set in the support field.
 *
 * @note Please ensure the `should_fallback_support` field is checked
 * before calling this function, this function DOES NOT check for for
 * this feature, and will give incorrect results if the field is not
 * checked PRIOR.
 *
 * @param mode The mode to check.
 * @param support The support field of the instruction.
 * @return True if the mode is matched, otherwise false.
 *
 * @see instr_support_t
 */
bool mode_valid(const enum modes mode, const instr_support_t support);

#endif