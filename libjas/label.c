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

#include "label.h"
#include "dir.h"
#include "error.h"
#include "operand.h"
#include <stdlib.h>
#include <string.h>

uint8_t label_create(label_table_t *label_table, label_t input) {
  if (label_lookup(label_table, input.name)) {
    err("duplicated labels");
    return 1;
  }

  label_table->size++;

  size_t size = label_table->size * sizeof(label_t);
  label_table->entries = realloc(label_table->entries, size);
  label_table->entries[label_table->size - 1] = input;

  return 0;
}

label_t *label_lookup(label_table_t *label_table, char *name) {
  label_t *table_defref = label_table->entries;

  for (size_t i = 0; i < label_table->size; i++) {
    if (table_defref[i].name == NULL) return NULL;

    if (strcmp(table_defref[i].name, name) == 0)
      return &table_defref[i];
  }
  return NULL;
}

#define is_word(x) (x < INT16_MAX && x > INT16_MIN)

enc_serialized_instr_t *label_evaluate(
    enc_serialized_instr_t *instr,
    uint64_t current, uint64_t label, enum modes mode) {

  bool is_mem =
      instr->has_modrm && (instr->modrm.mod == 0b11) &&
      ((instr->modrm.rm == 5) || (instr->modrm.rm == 6));

  int64_t effective_offset = label;
  if (mode != MODE_LONG && is_mem) goto default;

  uint8_t instr_size =
      instr->prefixes.len +
      instr->rex + instr->opcode_size +
      instr->has_modrm + instr->has_sib +
      instr->disp_size + instr->imm_size;

  /// @note how an offset of 0x00 is a valid offset of the
  /// next instruction **following** the current instruction
  /// when addressing via RIP in long mode.
  effective_offset = label - (current + instr_size);

default:
  instr->disp_size = is_mem ? 4 : instr->disp_size;
  instr->disp = (int64_t)effective_offset;
  if (mode == MODE_LONG || !is_mem) return instr;

  bool offset_is_word = is_word(effective_offset);
  buffer_t *pre = &instr->prefixes;

  switch (mode) {
  case MODE_REAL:
    if (offset_is_word) break;

    if (!buf_element_exists(pre, OP_ADDR_OVERRIDE))
      buf_write_byte(pre, OP_ADDR_OVERRIDE);

    instr->disp_size = 4; // `enc_serialize` presets `2`.
    instr->modrm.rm = 5;  // `5` is default across x86.

    break;

  case MODE_PROTECTED:
    if (!offset_is_word) break;

    instr->disp_size = 2; // Overrides value of 4 as initalized
    instr->modrm.rm = 6;

    // Overrides offset where the hasn't already been:
    if (!buf_element_exists(pre, OP_ADDR_OVERRIDE))
      buf_write_byte(pre, OP_ADDR_OVERRIDE);

    break;
  }
  return instr;
}
#undef is_word

static void add_call(char *name, label_t **table, size_t count) {
  label_t *label = (label_t *)NULL;

  if (!label_lookup(*table, name)) {
    label_t temp = (label_t){.name = name};

    label_create(table, temp);
    label = label_lookup(*table, name);
  }

  label->callers.count++;

  size_t s = label->callers.count * sizeof(size_t);
  label->callers.offsets = realloc(label->callers.offsets, s);
  label->callers.offsets[label->callers.count - 1] = count;
}

label_table_t label_table_gen(
    enc_serialized_instr_t *instr, // Should be padded!
    instr_generic_t *generics, size_t s) {

  label_table_t table = (label_table_t){0};
  size_t counter = 0;

  for (size_t i = 0; i < s; i++) {
    if (generics[i].type == DIRECTIVE) {
      directive_t curr_directive = generics[i].dir;

      // clang-format off
        if (curr_directive.dir == DIR_DEFINE_LABEL) {
          label_t *prev = 
            label_lookup(table, curr_directive.label.name);
          
          if (prev) {
            prev->address = counter;
            prev->type = curr_directive.label.type;
          } else {
            label_t processed_label = curr_directive.label;
            processed_label.address = counter;
            label_create(&table, processed_label);
          }
        }
        else { counter += curr_directive.data.len; }
        continue; // clang-format on
    }

    /// @note assumption that instructions that pass the
    /// previous guard is now considered to posses a type
    /// of `INSTRUCTION`.

    for (uint8_t j = 0; j < 4; j++) {
      operand_t curr_op = generics[i].instr.operands[j];
      char *label_name = NULL;

      if (!curr_op.type) break;

      if (op_rel(curr_op.type)) label_name = curr_op.label;
      if (op_m(curr_op.type) && curr_op.mem.src_type == LABEL)
        label_name = curr_op.mem.src.label;

      add_call_entry(label_name, table, counter);
    }

    // Messy means to calculate instruction size.
    counter += instr[i].prefixes.len + instr[i].rex;
    counter += instr[i].opcode_size;
    counter += instr[i].has_modrm + instr[i].has_sib;
    counter += instr[i].disp_size + instr[i].imm_size;
  }

  for (size_t i = 0; i < table.size; i++) {
    // Since `LABEL_NULL` is denoted as a raw value of 0:
    if ((uint8_t)table.entries[i].type == 0) {
      label_free_table(table);

      err("undefined label");
      return (label_table_t){0};
    }
  }
  return table;
}

instr_generic_t *label_gen(char *name, enum label_type type) {
  label_t label_instance = (label_t){0};
  label_instance.type = type;

  const size_t label_name_size = strlen(name) + 1;
  char *copied_name = malloc(label_name_size);
  strcpy(copied_name, label_instance.name);

  instr_generic_t *instr_ret = malloc(sizeof(instr_generic_t));

  *instr_ret = (instr_generic_t){
      .type = DIRECTIVE,
      .dir = (directive_t){
          .dir = DIR_DEFINE_LABEL,
          .label = label_instance,
      },
  };

  return instr_ret;
}

void label_free(label_t *label) {
  if (label->callers.count > 0) {
    for (size_t i = 0; i < label->callers.count; i++)
      free(label->callers.offsets[i]);
  }

  free(label);
}