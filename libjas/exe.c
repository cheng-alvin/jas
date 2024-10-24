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
#include "endian.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * @brief File with functions for generating the parts of the
 * ELF executable file format. Jas is only implementing x64
 * linux ELF files, which will be implemented in the following
 * order:
 *
 * - ELF header - ✅
 * - Section headers
 * - String table
 * - Text section
 * - Symbol table
 *
 * - Relocation table
 */

buffer_t exe_header(size_t sect_start, uint16_t sect_count, uint16_t sect_count_str) {
  buffer_t ret = BUF_NULL;
  size_t long_pad = 0;
  int int_pad = 0;

  const uint8_t magic_num[] = {0x7f, 'E', 'L', 'F'};
  buf_write(&ret, magic_num, sizeof(magic_num)); // Magic number

  /**
   * @note The following fields are assumed to be x64 linux specific
   * and are not portable across different operating systems or
   * architectures, due to the supported archs of the jas project.
   */

  buf_write_byte(&ret, 2);    // Operating mode
  buf_write_byte(&ret, 1);    // Endian-ness
  buf_write_byte(&ret, 1);    // ELF version
  buf_write_byte(&ret, 0x03); // Target ABI
  buf_write_byte(&ret, 0);    // ABI version

  buf_write(&ret, (uint8_t[]){0, 0, 0, 0, 0, 0, 0}, 7); // Padding - 7 byte

  const uint8_t *type = endian((uint8_t[]){0x00, 0x01}, 2);
  buf_write(&ret, type, sizeof(type)); // Executable type type
  buf_write(&ret, type, sizeof(type)); // Machine ISA

  free((void *)type);

  buf_write(&ret, endian((uint8_t[]){0x00, 0x00, 0x00, 0x01}, 4), 4); // ELF version

  buf_write(&ret, (uint8_t *)&long_pad, 8);   // Entry point
  buf_write(&ret, (uint8_t *)&long_pad, 8);   // Program header table offset
  buf_write(&ret, (uint8_t *)&sect_start, 8); // Section header table offset

  buf_write(&ret, (uint8_t *)&int_pad, 4);                // Flags - Not used
  buf_write(&ret, endian((uint8_t[]){0x40, 0x40}, 2), 2); // ELF header size

  /**
   * @note The program header table is not used in this implementation of the ELF
   * file format, hence the program header table is only used when a program is
   * standalone and not a shared object or a relocatable file.
   */

  buf_write(&ret, (uint8_t *)&int_pad, 4); // Program header table entry size and count - Combined not used

  //! Note: In small endian form, HARD CODED at this time
  buf_write(&ret, (uint8_t[]){0x40, 0x00}, 2);    // Section header table entry size
  buf_write(&ret, (uint8_t *)&sect_count, 2);     // Section header table count
  buf_write(&ret, (uint8_t *)&sect_count_str, 2); // Section header table string table index

  return ret;
}

// buffer_t exe_sect_header(uint32_t str_offset){
// buf_write((uint8_t*)&str_offset, 4); // String table name offset
// buf_write()
// }

