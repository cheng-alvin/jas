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

#include "d.h"
#include "endian.h"
#include "error.h"
#include "label.h"

void d(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  /**
   * @brief This opcode identity should:
   *   1. Write the opcode to the buffer ✅
   *   2. Calculate the relative offset of the label ✅
   *   3. Write the relative offset to the buffer
   */

  // IN BYTES!!!
  const uint8_t rel_sz = op_sizeof(op_arr[0].type) / 8;

  if (rel_sz == 8) {
    err("A relative address cannot be 64 bit-sized.");
    return;
  }

  check_mode(mode, instr_ref->support);
  buf_write(buf, OP_OPCODE_HELPER, instr_ref->opcode_size);

  // Calculate the relative offset of the label

  label_t *label = label_lookup((char *)op_arr[0].data);
  if (!label) {
    err("Referenced label was not found.");
    return;
  }

  // WORKS???
  buf_write(buf, endian(buf->len - label->address, rel_sz), rel_sz);
}
