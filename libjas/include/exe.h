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

#include "buffer.h"
#include <stddef.h>
#include <stdint.h>

/**
 * Function for generating the ELF header, targeted only towards x64
 * 64-bit ELF relocatable executables. Generates only the header of
 * the file and subsequent stuff can be found below.
 *
 * @param sect_start The starting address of the section
 * @param sect_count The number of sections in the ELF file
 * @param sect_count_str The index of the string table section
 *
 * @return The buffer containing the ELF header
 */
buffer_t exe_header(size_t sect_start, uint16_t sect_count, uint16_t sect_count_str);

/**
 * Function for generating the section header of the ELF object file.
 * And can be used in conjunction with the `exe_header` function and
 * other utilities to generate the entire ELF object file.
 *
 * @param str_offset The offset of the string table
 * @param type The type of the section
 * @param flags The flags of the section
 * @param off The offset of the section
 * @param sect_sz The size of the section
 *
 * @return The buffer containing the section header
 *
 * @note Specific values and flags can be found in the linux source
 * tree as well as the POSIX ELF standard.
 *
 * @see https://github.com/torvalds/linux/blob/master/include/uapi/linux/elf.h
 */
buffer_t exe_sect_header(uint32_t str_offset, uint32_t type, uint64_t flags, uint64_t off, uint64_t sect_sz);