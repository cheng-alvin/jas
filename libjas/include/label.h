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

#ifndef LABEL_H
#define LABEL_H

#include <stdbool.h>
#include <stddef.h>

typedef struct instr_generic instr_generic_t;

typedef struct {
  char *name;     /* Name of the label in a string format */
  bool exported;  /* Boolean for whether the label is exported to the linker table or not */
  bool ext;       /* Boolean for whether the label is external or not (If a relocation table should be created) */
  size_t address; /* Address of the label entry, can use `buf.len` */
} label_t;

/**
 * A factory function for creating a label entry in the label table.
 * The label entry is a struct that contains the name of the label,
 * whether it is exported or not, whether it is external or not, and
 * the address of the label. This data is constructed and then stored
 * into the given table pointer.
 *
 * @param label_table The label table to store the label entry in.
 * @param label_table_size The size of the label table.
 *
 * @param name The name of the label.
 * @param exported Boolean for whether the label is exported or not.
 * @param ext Boolean for whether the label is external or not.
 * @param address The address of the label.
 */
void label_create(
    label_t **label_table, size_t *label_table_size,
    char *name, bool exported, bool ext, size_t address);

/**
 * Function for destroying the label table, freeing the memory
 * allocated for the label names and the label entries itself.
 *
 * @param label_table The label table to destroy.
 * @param label_table_size The size of the label table
 */
void label_destroy_all(label_t **label_table, size_t *label_table_size);

/**
 * Function for looking up a label in the label table, and retu-
 * rning the label entry if found, otherwise returning a `NULL`
 * label entry will be returned back to the caller.
 *
 * @param label_table The label table to look up the label in.
 * @param label_table_size The size of the label table.
 *
 * @param name The name of the label to look up in the label table.
 * @return The pointer to the label entry if found, otherwise `NULL`
 *
 * @note The label table is **assumed** to be allocated and setup
 * correctly before calling this function, stored in the `label_table`
 * and `label_table_size` pointers (as shown above).
 */
label_t *label_lookup(label_t **label_table, size_t *label_table_size, char *name);

/**
 * Enumeration for expressing the different types of labels used
 * in the assembler, such as local, global, and external labels.
 *
 * Determines the type of label to be created in the assembler.
 * (And if a label table should be generated)
 */

enum label_type {
  LABEL_LOCAL,
  LABEL_GLOBAL,
  LABEL_EXTERN,
};

/**
 * Function for generating an instruction generic structure for the
 * represented label, with relative ease of use. This function would
 * automatically create the instruction generic, leading to an instr-
 * uction directive.
 *
 * @param name Name of the label to be created.
 * @param type The permissions of this label, as defined in the OS.
 *
 * @return The instruction generic pointer: @note should be freed
 */
instr_generic_t *label_gen(char *name, enum label_type type);

#endif