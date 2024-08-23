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

#ifndef ENDIAN_H
#define ENDIAN_H

#include <stddef.h>
#include <stdint.h>

/**
 * Function for converting a byte array from one endian
 * to the opposite endian mode.
 *
 * @param data The byte array to convert.
 * @param data_size The size of the byte array.
 *
 * @return The converted byte array.
 *
 * @note Array was returned using `malloc`ed memory,
 * therefore, must the freed after usage!
 */
uint8_t *endian(uint8_t *data, size_t data_size);

/**
 * Wrapper function for `endian` to convert a custom
 * integer to the opposite endian mode with the same
 * output format as the traditional `endian` function.
 *
 * (Since the assembler accepts the quad-word-sized
 * data for operand offsets or immediate values etc)
 *
 * @param data The input integer to convert.
 * @param data_size The size of the integer (data).
 *
 * @note `data_size` shall not be confused with the
 * array size as per `endian` function, it's the size
 * of the integer in bytes as opposed to the array size.
 * (e.g 8 bytes for quad-word)
 *
 * @return The converted byte array.
 *
 * @note Array was returned using allocated memory,
 * therefore, must the freed after usage! (and it's
 * ur responsibility to free it, don't forget!)
 *
 * @note No error handling is implemented in this function.
 * (e.g. if the data_size is incorrect, it will do anything
 * you throw at it, throwing some garbage back at you or a
 * segfault, so be careful!)
 */
uint8_t *sized_endian(void *data, uint8_t data_size);

#endif