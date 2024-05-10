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

#include "mr.h"
#include "buffer.h"
#include "error.h"
#include "operand.h"
#include <stdint.h>

// Static definitions
static int8_t operand_mode(const operand_t *op_arr);

void mr(const operand_t *op_arr, const buffer_t *buf, __attribute__((__unused)) const instr_encode_table_t *instr_ref) {
  const uint8_t *reg = op_arr[0].data;
  const uint8_t *rm = op_arr[1].data;
  const int8_t mode = operand_mode(op_arr);

  if (mode == -1)
    return;

  buf_write(buf, mode << 6 | *reg << 3 | *rm, 1);
}

static int8_t operand_mode(const operand_t *op_arr) {
  if (op_r(op_arr[0].type))
    return OP_MODRM_REG;
  else if (op_m(op_arr[0].type))
    return OP_MODRM_INDIRECT;

  else {
    err("Operand identity mismatch. (hint: displacements are not supported yet)");
    return -1;
  }
}
