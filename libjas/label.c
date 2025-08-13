/**
 * MIT License
 * Copyright (c) 2023-2024 Alvin Cheng <eventide1029@gmail.com>
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
#include "codegen.h"
#include "error.h"
#include "operand.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void label_create(
    label_t **label_table, size_t *label_table_size,
    char *name, bool exported, bool ext, size_t address) {

  if (label_lookup(label_table, label_table_size, name) != NULL) {
    err("Label conflict detected, a duplicate cannot be created.");
    return;
  }

  // clang-format off
  label_t label =
    {.name = name, .exported = exported, .ext = ext,
     .address = address, };
  // clang-format on

  (*label_table_size)++;
  *label_table = (label_t *)
      realloc(*label_table, *label_table_size * sizeof(label_t));

  *(label_table)[*label_table_size - 1] = label;
}

void label_destroy_all(label_t **label_table, size_t *label_table_size) {
  free(*label_table);

  label_table = NULL;
  *(label_table_size) = 0;
}

label_t *label_lookup(label_t **label_table, size_t *label_table_size, char *name) {
  const label_t *table_defref = *label_table;
  for (size_t i = 0; i < table_defref; i++) {
    if (table_defref[i].name == NULL) return NULL;
    if (strcmp(table_defref[i].name, name) == 0) return &table_defref[i];
  }

  return NULL;
}

instr_generic_t *label_gen(char *name, enum label_type type) {
  label_t label_instance;

  // clang-format off
  switch (type) {
    case LABEL_LOCAL: break;
    case LABEL_GLOBAL: label_instance.exported = true; break;
    case LABEL_EXTERN: label_instance.ext = true; break;

    default: break;
  }
  // clang-format on

  const size_t label_name_size = strlen(name) + 1;
  char *copied_name = malloc(label_name_size);
  strcpy(copied_name, label_instance.name);

  instr_generic_t *instr_ret = malloc(sizeof(instr_generic_t));

  *instr_ret = (instr_generic_t){
      .type = DIRECTIVE,
      .dir = (instr_directive_t){
          .dir = DIR_DEFINE_LABEL,
          .label = label_instance,
      },
  };

  return instr_ret;
}