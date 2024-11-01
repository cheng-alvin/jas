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

#ifndef CODEGEN_H
#define CODEGEN_H

#include "buffer.h"
#include "instruction.h"
#include "mode.h"
#include <stddef.h>

enum codegen_modes {
  CODEGEN_RAW,
  CODEGEN_ELF,
};

/**
 * Codegen function that generates the machine code from the
 * instruction array. Basically a wrapper for a bunch of lib-
 * jas sub-modules function.
 *
 * @param mode The mode to generate the machine code in
 * @param instr_arr The instruction array to generate the code from
 * @param arr_size The size of the instruction array (in bytes)
 * @param exec_mode The output mode of the codegen function
 * (i.e raw code or elf)
 *
 * @return The buffer struct containing the machine code
 *
 * @see `buffer.h`
 * @see `instruction.h`
 * @see `codegen_modes`
 */
buffer_t codegen(enum modes mode, instruction_t *instr_arr, size_t arr_size, enum codegen_modes exec_mode);

#endif
