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

#include "operand.h"
#include "error.h"
#include "register.h"
#include "rex.h"
#include <stdbool.h>
#include <stdint.h>

uint8_t op_sizeof(enum operands input) {
  if (op_byte(input))
    return 8;

  if (op_word(input))
    return 16;

  if (op_dword(input))
    return 32;

  if (op_qword(input))
    return 64;

  return 0;
}

buffer_t op_write_prefix(const operand_t *op_arr) {
  buffer_t prefix = BUF_NULL;
  uint8_t rex = REX_DEFAULT;

  for (uint8_t i = 0; i < 4; i++) {
    const uint8_t size = op_sizeof(op_arr[i].type);
    uint8_t override = op_m(op_arr[i].type) ? OP_ADDR_OVERRIDE : OP_WORD_OVERRIDE;

    if (op_rm(op_arr[i].type))
      rex |= reg_needs_rex(op_arr[i].data) ? REX_B : 0;

    switch (size) {
    case 16:
      if (!buf_element_exists(&prefix, override))
        buf_write_byte(&prefix, override);
      break;

    case 64:
      if (!buf_element_exists(&prefix, REX_W))
        rex |= REX_W;
      break;

    default:
      break;
    }
  }

  buf_write_byte(&prefix, rex);
  return prefix;
}

operand_t op_construct_operand(enum operands type, size_t offset, void *data) {
  return (operand_t){
      .type = type,
      .offset = offset,
      .data = data,
  };
}