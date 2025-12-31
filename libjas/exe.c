/**
 * MIT License
 * Copyright (c) 2023-2026 Alvin Cheng <eventide1029@gmail.com>
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

#define STD_UNIT_SIZE \
  (uint8_t[]){0x40, 0x00}

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

  buf_write(&ret, (uint8_t *)&long_pad, 7); // Padding - Takes 7 bytes off 8

  buf_write(&ret, (uint8_t[]){0x01, 0x00}, 2); // Executable type type
  buf_write(&ret, (uint8_t[]){0x3E, 0x00}, 2); // Machine ISA

  buf_write(&ret, (uint8_t[]){0x01, 0x00, 0x00, 0x00}, 4); // ELF version

  buf_write(&ret, (uint8_t *)&long_pad, 8);   // Entry point
  buf_write(&ret, (uint8_t *)&long_pad, 8);   // Program header table offset
  buf_write(&ret, (uint8_t *)&sect_start, 8); // Section header table offset

  buf_write(&ret, (uint8_t *)&int_pad, 4); // Flags - Not used
  buf_write(&ret, STD_UNIT_SIZE, 2);       // ELF header size

  /**
   * @note The program header table is not used in this implementation of the ELF
   * file format, hence the program header table is only used when a program is
   * standalone and not a shared object or a relocatable file.
   */
  buf_write(&ret, (uint8_t *)&int_pad, 4); // Program header table entry size and count - Combined not used

  buf_write(&ret, STD_UNIT_SIZE, 2);              // Section header table entry size
  buf_write(&ret, (uint8_t *)&sect_count, 2);     // Section header table count
  buf_write(&ret, (uint8_t *)&sect_count_str, 2); // Section header table string table index

  return ret;
}

buffer_t exe_sect_header(uint32_t str_offset, uint32_t type, uint64_t flags, uint64_t *off, uint64_t sect_sz, size_t info, uint32_t label_sect, uint64_t ent_size) {
  const uint64_t long_pad = 0;
  buffer_t ret = BUF_NULL;

  buf_write(&ret, (uint8_t *)&str_offset, 4); // String table name offset
  buf_write(&ret, (uint8_t *)&type, 4);       // Section type
  buf_write(&ret, (uint8_t *)&flags, 8);      // Section flags

  buf_write(&ret, (uint8_t *)&long_pad, 8); // Section address

  buf_write(&ret, (uint8_t *)off, 8);      // Section file offset
  buf_write(&ret, (uint8_t *)&sect_sz, 8); // Section size
  *off += sect_sz;

  // Maybe padded:
  buf_write(&ret, &label_sect, 4);          // Section link
  buf_write(&ret, &info, 4);                // Section info
  buf_write(&ret, (uint8_t *)&long_pad, 8); // Section address alignment
  buf_write(&ret, &ent_size, 8);            // Section entry size

  return ret;
}

buffer_t exe_sym_ent(char *name, uint64_t sym_val, uint16_t sect_idx, buffer_t *strtab, uint8_t info) {
  buffer_t symtab = BUF_NULL;
  uint64_t long_pad = 0;

  buf_write(&symtab, (uint32_t *)&strtab->len, 4); // Name offset
  buf_write_byte(&symtab, info);                   // Info
  buf_write_byte(&symtab, 0);                      // Other
  buf_write(&symtab, &(uint16_t){sect_idx}, 2);    // Section index
  buf_write(&symtab, &sym_val, 8);                 // Value
  buf_write(&symtab, (uint8_t *)&long_pad, 8);     // Size

  buf_write(strtab, (uint8_t *)name, strlen(name) + 1); // Add name to string table

  return symtab;
}

buffer_t exe_generate(struct codegen_ret ret) {
  uint8_t *p_padding = calloc(1, 0x40);
  buffer_t result_buffer = BUF_NULL;

  /// @note header size of 0x40
  size_t addr = 0x40 * 6;

  // Data has been HARD-CODED rather than messing around with
  // OOP and structs, we only need these for now, right? Add more.
  const char shstrtab[] = "\0.shstrtab\0.strtab\0.symtab\0.text\0";
  buffer_t shstrtab_head =
      exe_sect_header(1, 3, 2, &addr, sizeof(shstrtab), 0, 0, 0);

  buffer_t strtab = BUF_NULL;
  buffer_t symtab = BUF_NULL;

  for (size_t i = 0; i < ret.label_table_size; i++) {
    // Truncating 24 bytes off the 64-byte allocated buffer using `calloc`.
    buf_write(&symtab, p_padding, 24);

    const char *name = ret.label_table[i].name;
    buf_write(&strtab, name, strlen(ret.label_table[i].name) + 1);

    uint8_t binding = 0;
    label_t curr_label = ret.label_table[i];
    if (label_exported(curr_label) || label_extern(curr_label)) binding = 1;

    const buffer_t ent = exe_sym_ent(curr_label.name, curr_label.address, 4,
                                     &strtab, (((binding) << 4) + ((0) & 0xf)));

    buf_concat(&symtab, 1, ent);
    free(ent.data);
  }

  const size_t info = ret.label_table_size + 1;
  buffer_t text_head = exe_sect_header(27, 1, 7, &addr, ret.code.len, 0, 0, 0);
  buffer_t strtab_head = exe_sect_header(11, 3, 2, &addr, strtab.len, 0, 0, 0);
  buffer_t symtab_head =
      exe_sect_header(19, 2, 2, &addr, symtab.len, ret.label_table_size, 2, 24);

  // Adjust the amount of sections as required.
  const buffer_t header = exe_header(0x40, 5, 1);
  buf_write(&result_buffer, header.data, header.len);
  buf_write(&result_buffer, p_padding, 0x40);

#define FREE_ALL(...)                        \
  do {                                       \
    void *p[] = {__VA_ARGS__};               \
    size_t len = sizeof(p) / sizeof(void *); \
    for (size_t i = 0; i < len; ++i)         \
      free(p[i]);                            \
  } while (0)

  buf_concat(&result_buffer, 4, shstrtab_head, strtab_head, symtab_head, text_head);
  buf_write(&result_buffer, shstrtab, sizeof(shstrtab));
  buf_concat(&result_buffer, 3, strtab, symtab, ret.code);

  free(p_padding);

  // clang-format off

  FREE_ALL(
    shstrtab_head.data, strtab.data, symtab.data, strtab_head.data, 
    symtab_head.data, text_head.data, header.data
  );

  // clang-format on

  return result_buffer;
}