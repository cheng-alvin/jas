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

#ifndef REX_H
#define REX_H

#include "buffer.h"
#include <stdint.h>

/**
 * Macros for defining the REX byte values. These values
 * are used to override operand sizes when using long mode.
 * A letter is followed after the `REX_` prefix to dictate the
 * prefix type used in the situation, and can be `|`ed
 * together as well!
 *
 * @see https://wiki.osdev.org/X86-64_Instruction_Encoding#REX_prefix
 */

#define REX_W 0x48
#define REX_R 0x44
#define REX_X 0x42
#define REX_B 0x41

#define REX_DEFAULT 0b01000000

/**
 * Function for inserting a REX byte into the instruction buffer,
 * dynamically. (Wow fancy!) This function will just simply check
 * if there is another old REX byte in the buffer, and if there is,
 * it will just create a new one!
 *
 * @param buf The buffer to insert the REX byte into.
 * @param rex The REX byte to insert into the buffer.
 *
 * :)
 */
void rex_insert(buffer_t *buf, const uint8_t rex);

#endif
