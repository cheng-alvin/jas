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

#include "label.h"
#include "dir.h"
#include "error.h"
#include "operand.h"
#include <stdlib.h>
#include <string.h>

void label_create(label_table_t *label_table, label_t input) {
  if (label_lookup(label_table, input.name))
    return err("duplicated labels");

  label_table->size++;

  size_t size = label_table->size * sizeof(label_t);
  label_table->entries = realloc(label_table->entries, size);
  label_table->entries[label_table->size - 1] = input;
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
