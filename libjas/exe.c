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

#include "exe.h"
#include "endian.h"
#include "label.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief File with functions for generating the parts of the
 * ELF executable file format. Jas is only implementing x64
 * linux ELF files, which will be implemented in the following
 * order:
 *
 * - ELF header - ✅
 * - Section headers - ✅
 * - String table - ✅
 * - Text section - ✅
 * - Symbol table - ✅
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

  buf_write(&ret, (uint8_t[]){0, 0, 0, 0, 0, 0, 0}, 7); // Padding - 7 bytes

  buf_write(&ret, endian((uint8_t[]){0x00, 0x01}, 2), 2); // Executable type type
  buf_write(&ret, endian((uint8_t[]){0x00, 0x3E}, 2), 2); // Machine ISA

  buf_write(&ret, endian((uint8_t[]){0x00, 0x00, 0x00, 0x01}, 4), 4); // ELF version

  buf_write(&ret, (uint8_t *)&long_pad, 8);   // Entry point
  buf_write(&ret, (uint8_t *)&long_pad, 8);   // Program header table offset
  buf_write(&ret, (uint8_t *)&sect_start, 8); // Section header table offset

  buf_write(&ret, (uint8_t *)&int_pad, 4);                // Flags - Not used
  buf_write(&ret, endian((uint8_t[]){0x00, 0x40}, 2), 2); // ELF header size

  /**
   * @note The program header table is not used in this implementation of the ELF
   * file format, hence the program header table is only used when a program is
   * standalone and not a shared object or a relocatable file.
   */

  buf_write(&ret, (uint8_t *)&int_pad, 4); // Program header table entry size and count - Combined not used

  buf_write(&ret, (uint8_t[]){0x40, 0x00}, 2);    // Section header table entry size
  buf_write(&ret, (uint8_t *)&sect_count, 2);     // Section header table count
  buf_write(&ret, (uint8_t *)&sect_count_str, 2); // Section header table string table index

  return ret;
}

#define QWORD_PAD \
  &(uint64_t) { 0 }

buffer_t exe_sect_header(uint32_t str_offset, uint32_t type, uint64_t flags, uint64_t *off, uint64_t sect_sz) {
  buffer_t ret = BUF_NULL;
  buf_write(&ret, (uint8_t *)&str_offset, 4); // String table name offset
  buf_write(&ret, (uint8_t *)&type, 4);       // Section type
  buf_write(&ret, (uint8_t *)&flags, 8);      // Section flags

  buf_write(&ret, QWORD_PAD, 8); // Section address

  buf_write(&ret, (uint8_t *)off, 8);      // Section file offset
  buf_write(&ret, (uint8_t *)&sect_sz, 8); // Section size
  *off += sect_sz;

  int int_pad = 0;

  if (type == 0x02) {
    buf_write(&ret, &(uint32_t){2}, 4);                    // Section link
    buf_write(&ret, &(uint32_t){label_get_size() + 1}, 4); // Section info
    buf_write(&ret, QWORD_PAD, 8);                         // Section address alignment
    buf_write(&ret, &(uint64_t){0x18}, 8);                 // Section entry size

    return ret;
  }

  buf_write(&ret, (uint8_t *)&int_pad, 4); // Section link
  buf_write(&ret, (uint8_t *)&int_pad, 4); // Section info
  buf_write(&ret, QWORD_PAD, 8);           // Section address alignment
  buf_write(&ret, QWORD_PAD, 8);           // Section entry size

  return ret;
}

buffer_t exe_sym_ent(char *name, uint64_t sym_val, uint16_t sect_idx, buffer_t *strtab, uint8_t info) {
  buffer_t symtab = BUF_NULL;

  buf_write(&symtab, (uint32_t *)&strtab->len, 4); // Name offset
  buf_write_byte(&symtab, info);                   // Info
  buf_write_byte(&symtab, 0);                      // Other
  buf_write(&symtab, &(uint16_t){sect_idx}, 2);    // Section index
  buf_write(&symtab, &sym_val, 8);                 // Value
  buf_write(&symtab, QWORD_PAD, 8);                // Size

  buf_write(strtab, (uint8_t *)name, strlen(name) + 1); // Add name to string table

  return symtab;
}