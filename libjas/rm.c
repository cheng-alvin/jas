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

#include "rm.h"
#include "buffer.h"
#include "error.h"
#include "mode.h"
#include "operand.h"
#include "register.h"
#include "rex.h"
#include <stdint.h>

void rm(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  const enum registers *reg = (enum registers *)op_arr[0].data;
  const uint8_t rm = reg_lookup_val(op_arr[1].data);

  const buffer_t prefixes = op_write_prefix(op_arr);
  buf_write(buf, prefixes.data, prefixes.len);
  free(prefixes.data);
  check_mode(mode, instr_ref->support);
  buf_write(buf, OP_OPCODE_HELPER, instr_ref->opcode_size);

  if (reg_needs_rex(*reg))
    rex_insert(buf, REX_B);

  uint8_t mr_mode = OP_MODRM_REG;

  if (op_m(op_arr[1].type) && op_arr[1].offset == 0)
    mr_mode = OP_MODRM_INDIRECT;
  else if (op_arr[1].offset != 0)
    mr_mode = OP_MODRM_DISP8;

  buf_write_byte(buf, mr_mode << 6 | *reg << 3 | rm);

  if (op_arr[1].offset != 0)
    buf_write_byte(buf, op_arr[1].offset);
}
