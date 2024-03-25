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

#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint8_t *data; /* Buffer data */
  size_t len;    /* Length of buffer */
} buffer_t;

/**
 * Writes data to the buffer struct with the `data_len` bytes
 * while reallocating and allocating the buffer if necessary.
 *
 * @param buf The pointer to the buffer struct
 * @param data The pointer to the data to write
 * @param data_len The length of the data to write
 */
void buf_write(buffer_t *buf, const uint8_t *data, const size_t data_len);

/**
 * Removes an element from the buffer struct at the specified index.
 *
 * @param buf The pointer to the buffer struct
 * @param elem The index of the element to remove
 */
void buf_remove(buffer_t *buf, const size_t elem);

/**
 * Removes a chunk of elements from the buffer struct starting from the
 * `start` index to the `end` index.
 *
 * @param buf The pointer to the buffer struct
 * @param start The starting index of the chunk to remove
 * @param end The ending index of the chunk to remove
 */
void buf_remove_chunk(buffer_t *buf, const size_t start, const size_t end);

#endif