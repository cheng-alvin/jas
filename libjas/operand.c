/**
 * MIT License
 * Copyright (c) 2023-2025 Alvin Cheng <eventide1029@gmail.com>
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

#include "operand.h"
#include "error.h"
#include "mode.h"
#include "rex.h"
#include <limits.h>
#include <stdbool.h>

bool op_assert_types(operand_t *in, enum operands *ex, size_t sz) {
  for (size_t i = 0; i < sz; i++) {
    if (ex[i] == OP_NULL) break;
    if (in[i].type != ex[i]) return false;
  }

  return true;
}
// clang-format off
#define mode_return(size, mode) { if (sz) *sz = size; return mode; }

enum op_modrm_modes op_modrm_mode(uint64_t displacement, uint8_t *sz) {
  if (!displacement) mode_return(0, OP_MODRM_MODE_INDIRECT);
  if (displacement <= UCHAR_MAX) mode_return(1, OP_MODRM_MODE_DISP8);

  mode_return(4, OP_MODRM_MODE_DISP32);
}

#undef mode_return
// clang-format on

uint8_t op_sizeof(enum operands input) {
  if (op_byte(input)) return 8;
  if (op_word(input)) return 16;
  if (op_dword(input)) return 32;
  if (op_qword(input)) return 64;

  return 0;
}

void op_write_prefix(buffer_t *buf, operand_t *op_arr, enum modes mode) {
  uint8_t already_written = 0;

  for (uint8_t i = 0; i < 4; i++) {
    if (op_arr[i].type == OP_NULL)
      continue;

    const uint8_t size = op_sizeof(op_arr[i].type);
    const uint8_t override =
        op_m(op_arr[i].type) ? OP_ADDR_OVERRIDE : OP_WORD_OVERRIDE;

    switch (size) {
    case 16:
      if (mode == MODE_REAL) break;
      if (mode == MODE_LONG && op_m(op_arr[i].type)) {
        if (!buf_element_exists(buf, OP_WORD_OVERRIDE))
          buf_write_byte(buf, OP_WORD_OVERRIDE);
      }

      goto override_write;

    case 32:
      if (mode == MODE_REAL) goto override_write;
      if (override != OP_ADDR_OVERRIDE) break;
      if (mode == MODE_PROTECTED) break;

    override_write:
      if (already_written == override) break;
      buf_write_byte(buf, override);
      already_written = override;
      break;

    case 64:
      if (mode != MODE_LONG)
        err("64-bit operands not allowed");
    }
  }
}
