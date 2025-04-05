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

#ifndef CODEGEN_H
#define CODEGEN_H

#include "buffer.h"
#include "instruction.h"
#include "mode.h"
#include <stddef.h>

struct codegen_ret {
  buffer_t code;
  label_t *label_table;
  size_t label_table_size;
};

/**
 * Codegen function that generates the machine code from the
 * instruction array. Basically a wrapper for a bunch of lib-
 * jas sub-modules function.
 *
 * @param mode The mode to generate the machine code in
 * @param instr_input The instruction array to generate the code from
 * @param arr_count The size of the instruction array (in elements)
 *
 * @return The `codegen_ret` struct containing the machine code and meta-data.
 */
struct codegen_ret codegen(
    enum modes mode, instruction_t **instr_input, size_t arr_count);

/**
 * Wrapper function for the `codegen` function that gives boiler-
 * plate code to generate the binary of a single instruction given
 * in the instruction struct form, but not an array as seen in the
 * `codegen` function.
 *
 * @param mode The mode to generate the machine code in
 * @param instr The instruction pointer to generate the code from
 *
 * @return The buffer struct containing the machine code
 *
 * @see `codegen`
 */
buffer_t assemble_instr(enum modes mode, instruction_t *instr);

#endif
