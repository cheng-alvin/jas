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
#include "mode.h"
#include "operand.h"
#include "register.h"
#include "rex.h"
#include <stdint.h>

void mr(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  const uint8_t *reg = reg_lookup_val((enum registers)op_arr[0].data);
  const uint8_t *rm = op_arr[1].data;

  if (op_sizeof(op_arr[0].type) != op_sizeof(op_arr[1].type)) {
    err("Invalid operand sizes.");
    return;
  }

  op_write_prefix(buf, op_arr[0].type);

  if (reg_needs_rex((enum registers)op_arr[0].data)) {
    if (mode == MODE_LONG)
      buf->data[buf->len - 1] = buf->data[buf->len - 1] | REX_R;

    buf_write_byte(buf, REX_B);
  }

  if (instr_ref->should_fallback_support) {
    if (mode_valid(mode, instr_ref->support)) {
      err("Invalid operating mode.");
      return;
    }
  }

  uint8_t mr_mode;

  if (op_r(op_arr[0].type))
    mr_mode = OP_MODRM_REG;
  else if (op_m(op_arr[0].type))
    mr_mode = OP_MODRM_INDIRECT;

  else {
    err("Operand identity mismatch. (hint: displacements are not supported yet)");
    return;
  }

  buf_write_byte(buf, mr_mode << 6 | *reg << 3 | *rm);
}
