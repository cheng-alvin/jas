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

#include "buffer.h"
#include "error.h"
#include "instruction.h"
#include "mode.h"
#include "register.h"
#include <stddef.h>

// TODO Remember to note that NULL is when there's an error

buffer_t *codegen(enum modes mode, instruction_t *instr_arr) {
  /**
   * Implementation:
   * @brief takes an array of instructions and spits
   * out an array of binary values corresponding to
   * the imported instructions.
   *
   * TODO items (Tick when completed)
   * - Loop through the instruction array ✅
   * - Check and validate the instructions (责任交给了identity) ✅
   * - Dump opcodes & prefixes
   * - Dump returned operand encoding
   */

  for (int i = 0; i < sizeof(instr_arr); i++) {
    const instr_encode_table_t *instr = instr_table[instr_arr[i].instr];
  }
}
