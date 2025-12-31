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

#ifndef EXE_H
#define EXE_H

#include "buffer.h"
#include "codegen.h"
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
 * @param off The pointer to the offset of the section
 * @param sect_sz The size of the section
 * @param info Extra information (if applicable)
 * @param label_sect The section of label table are located.
 * @param ent_size The size of the entries in the content of this section.abort
 *
 * @note Please note the `ent_size` is used to depict the size of the
 * entries of the section content, which could be applicable in some
 * contexts such as a symbol *table* where each table has a certain size.
 *
 * @note Values of `label_sect`, `info` and `ent_size` are
 * optional and can be set to 0 if not applicable for certain contexts.
 *
 * @return The buffer containing the section header
 *
 * @note Specific values and flags can be found in the linux source
 * tree as well as the POSIX ELF standard.
 *
 * @see https://github.com/torvalds/linux/blob/master/include/uapi/linux/elf.h
 *
 * @note The `off` pointer is used to keep track of the offset/size of the section
 * headers and helps the caller to keep track of the section headers.
 */
buffer_t exe_sect_header(uint32_t str_offset, uint32_t type, uint64_t flags, uint64_t *off, uint64_t sect_sz, size_t info, uint32_t label_sect, uint64_t ent_size);

/**
 * Function for generating a symbol table entry in the ELF object file.
 * This function is used in conjunction with `exe_sect_header` function
 * for generating the entire symbol header and this is only used for
 * the individual symbol entries.
 *
 * @param name The name of the symbol
 * @param sym_val The value of the symbol (Which is the offset)
 * @param sect_idx The index of the section
 * @param strtab The string table buffer pointer
 * @param info The data for the corresponding info field
 *
 * @note The `strtab` buffer pointer is used as a descriptor for where the
 * symbol name shall be written and the index of the section is used to
 * determine the section the symbol is in.
 */
buffer_t exe_sym_ent(char *name, uint64_t sym_val, uint16_t sect_idx, buffer_t *strtab, uint8_t info);

buffer_t exe_generate(struct codegen_ret ret);

#endif