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

#include "label.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// TODO write tests
// WARNING: This is a stub implementation

static label_t *label_table = NULL;
static size_t label_table_size = 0;

void label_create(char *name, bool exported, bool ext, size_t address) {
  label_t label = {.name = name, .exported = exported, .ext = ext, .address = address};

  label_table_size++;
  label_table = (label_t *)realloc(label_table, label_table_size * sizeof(label_t));
  label_table[label_table_size - 1] = label;
}

void label_destroy_all() {
  free(label_table);

  label_table = NULL;
  label_table_size = 0;
}

label_t *label_lookup(char *name) {
  for (size_t i = 0; i < label_table_size; i++)
    if (strcmp(label_table[i].name, name) == 0)
      return &label_table[i];

  return NULL;
}

label_t *label_dump_all() {
  return label_table;
}