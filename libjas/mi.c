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

#include "mi.h"
#include "endian.h"
#include "error.h"
#include "operand.h"
#include "register.h"
#include "rex.h"
#include <stdlib.h>

void mi(operand_t *op_arr, buffer_t *buf, instr_encode_table_t *instr_ref, enum modes mode) {
  const uint8_t opcode_extend = instr_ref->opcode_ext << 3;
  const uint8_t rm = reg_lookup_val(op_arr[0].data);

  const buffer_t prefixes = op_write_prefix(op_arr, mode);
  buf_write(buf, prefixes.data, prefixes.len);
  free(prefixes.data);

  check_mode(mode, instr_ref->support);
  buf_write(buf, OP_OPCODE_HELPER, instr_ref->opcode_size);
  buf_write_byte(buf, op_modrm_mode(op_arr[0]) | opcode_extend | rm);

  if (op_arr[0].offset != 0)
    buf_write(buf, (uint8_t *)&op_arr[0].offset, 4);

  // ---

  if (op_arr[1].type == OP_IMM64) {
    err("Invalid immediate value.");
    return;
  }

  const uint8_t imm_size = op_sizeof(op_arr[1].type) / 8;
  uint8_t *imm = (uint8_t *)op_arr[1].data;
  buf_write(buf, imm, imm_size);
}
