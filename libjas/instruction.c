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

#include "instruction.h"
#include "mr.h"
#include "null.h"
#include "operand.h"
#include "rm.h"
#include <stdbool.h>

instr_encode_table_t mov[] = {
    {
        .ident = OP_MR,
        .opcode_ext = NULL,
        .opcode = {0x89},
        .support = NULL,
        .byte_instr_opcode = {0x88},
        .should_fallback_support = false,
    },
};

instr_encode_table_t *instr_table[] = {mov};

instr_encoder_t instr_encode_func(enum op_ident input) {
  instr_encoder_t lookup[] = {&mr, &rm};
  return lookup[(size_t)input];
}