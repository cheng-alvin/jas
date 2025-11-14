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

  size_t size = label_table->size * sizeof(label_t);
  label_table->entries = realloc(label_table->entries, size);

  label_table->size++;
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

instr_generic_t *label_gen(char *name, enum label_type type) {
  label_t label_instance;

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