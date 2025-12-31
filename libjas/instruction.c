/**
 * MIT License
 * Copyright (c) 2023-2026 Alvin Cheng <eventide1029@gmail.com>
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

#include "dir.h"
#include "error.h"
#include "operand.h"
#include "register.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// clang-format off

#define CURR_TABLE instr_table[(uint8_t)instr.instr]
instr_encode_table_t instr_get_tab(instruction_t instr) {
  #include "instructions.inc"
  // clang-format on

  for (uint8_t i = 1; CURR_TABLE[i].opcode_size; i++) {
    const enum operands operand_list[4] = {
        CURR_TABLE[i].operand_descriptors[0].type,
        CURR_TABLE[i].operand_descriptors[1].type,
        CURR_TABLE[i].operand_descriptors[2].type,
        CURR_TABLE[i].operand_descriptors[3].type,
    };

    bool pass = op_assert_types(&instr.operands, operand_list, 4);
    if (pass) return CURR_TABLE[i];
  }

  // fall-through; no corresponding instruction opcode found
  err("No corrsponding instruction opcode found.");
  return INSTR_TAB_NULL; // aka empty
}
#undef CURR_TABLE // As in not applicable in other contexts.

static void __instr_free__(instruction_t *instr) {
  for (uint8_t i = 0; (uint8_t)instr->operands[i].type; i++) {
    if (op_rel(instr->operands[i].type))
      free(instr->operands[i].label);

    char *mem_label = instr->operands[i].mem.src.label;
    if (instr->operands[i].mem.src_type == LABEL) free(mem_label);
  }
}

void instr_free(instr_generic_t *instr) {
  if (instr->type == INSTR) __instr_free__(&(instr->instr));

  if (instr->type == DIRECTIVE) {
    if (instr->dir.dir == DIR_DEFINE_LABEL) {
      if (strlen(instr->dir.label.name)) {
        const char *label_name = instr->dir.label.name;
        free(label_name);
      }
    }

    if (instr->dir.dir == DIR_DEFINE_BYTES) {
      const uint8_t buffer = instr->dir.data.data;
      free(buffer);
    }
  }

  free(instr); // All done now.
}

#define write_imm_data(type)                     \
  do {                                           \
    type type##_ = (type)va_arg(args, uint64_t); \
    operands[i].imm = (type)type##_;             \
  } while (0);

instr_generic_t *instr_gen(enum instructions instr, uint8_t operand_count, ...) {
  va_list args;
  va_start(args, operand_count * 3);

  // Note, a temporary register type is used to prevent conflict
  // with the `enum registers` type by passing into `write_imm_data`
  typedef enum registers temp_reg;

  if (operand_count > 4) {
    err("operand count must not exceed 4");
    return NULL;
  }

  operand_t operands[4] = {0};

  for (uint8_t i = 0; i < operand_count; i++) {
    const enum operands type = va_arg(args, enum operands);

    if (op_rel(type)) {
      char *lab = va_arg(args, char *);
      const size_t label_name_size = strlen(lab) + 1;
      char *copied_name = malloc(label_name_size);
      strcpy(copied_name, lab);

      operands[i].label = copied_name;

      // clang-format off
    } else if (op_imm(type)) {
      switch (op_sizeof(type)) {
      case 8: write_imm_data(uint8_t); break;
      case 16: write_imm_data(uint16_t); break;
      case 32: write_imm_data(uint32_t); break;
      case 64: write_imm_data(uint64_t); break;
      default:
        err("Invalid operand size.");
        break;
      }
      // clang-format on
    } else {
      operands[i].mem = va_arg(args, op_mem_t);
      operands[i].mem.disp = va_arg(args, uint64_t);
    }
  }

  va_end(args);

  instr_generic_t *instr_generic_ret = malloc(sizeof(instr_generic_t));

  *instr_generic_ret = (instr_generic_t){
      .type = INSTR,
      .instr = (instruction_t){.instr = instr},
  };

  memcpy(instr_generic_ret->instr.operands, operands, 4);
  return instr_generic_ret;
}
#undef alloc_data

instr_generic_t *instr_write_bytes(size_t data_sz, ...) {
  buffer_t *buffer_ptr = malloc(sizeof(buffer_t));
  buffer_t data = BUF_NULL;
  va_list args;
  va_start(args, data_sz);

  for (size_t i = 0; i < data_sz; i++) {
    const uint8_t byte = va_arg(args, uint8_t);
    buf_write_byte(&data, byte);
  }

  va_end(args);

  instr_generic_t *instr_ret = malloc(sizeof(instr_generic_t));

  *instr_ret = (instr_generic_t){
      .type = DIRECTIVE,
      .dir = (directive_t){
          .dir = DIR_DEFINE_BYTES,
          .data = data,
      },
  };
  return instr_ret;
}
