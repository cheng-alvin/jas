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
 * @see `LICENSE`
 */

#include "instruction.h"
#include <ctype.h>
#include <string.h>

/// @brief A bunch of utility functions where parsing of assembly
/// instructions are applicable, such as in assemblers.

/**
 * Function for parsing the instruction name and returning the
 * corresponding instruction enum. This is done by having a set
 * of all instruction names in order of the enum and comparing
 * the input name with the names in the set.
 *
 * @param name The name of the instruction to parse
 * @return The instruction enum
 */
enum instructions parse_instr_name(char *name);

/**
 * Converts a regular hex or base-10 string to a number, that
 * C can interpret. (a `uint64_t`) This is done by checking the
 * prefix of the string such as `0x` for hex and `h` for hex,
 * converting it using the C standard library function `strtoull`
 * and returning the result.
 *
 * @param name The string to convert to a number
 * @return The number) This
 */
uint64_t parse_str_num(char *name);

/**
 * Function for parsing the register name and returning the
 * corresponding register enum. This is done by having a set
 * of all register names in order of the enum and comparing
 * the input name with the names in the set.
 *
 * @param name The name of the register to parse
 * @return The register enum
 *
 * @note Implementation is similar to `parse_instr_name`
 * @see `register.h`
 */
enum registers parse_reg(char *name);