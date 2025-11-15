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

#ifndef LABEL_H
#define LABEL_H

#include <stdbool.h>
#include <stddef.h>

typedef struct instr_generic instr_generic_t;

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
  LABEL_EXTERN, // Associated with external linkage, TBD
};

typedef struct {
  /// @note Union discriminates between instruction indexes and data offsets
  /// depending on the label type. Encoder uses the defined`instruction_indexes`
  /// union element to track the exact struct acting as the caller of
  /// the label.

  union {
    uint64_t *instruction_indexes; /* List of callers of instruction indexes */
    size_t *data_offsets;          /* Callers aray in offset form  */
  } callers;

  char *name;     /* Name of the label in a string format */
  size_t address; /* Offset of label's definition relative to `.text` */

  enum label_type type; /* Type of the label */

  /// implementation @note The relative offset may be calculated
  /// during the encoding phase, through the use of `address`
  /// subtracted by the current caller's instruction pointer.
} label_t;

/**
 * Function for creating a new label entry in the label table,
 * handling required memory allocations and duplication checks.
 * However, this function does not exempt the caller's responsibility
 * of freeing the buffer after use.
 *
 * @param label_table The label table to store the label entry in.
 * @param input New table entry to be added to the label table.
 */

void label_create(label_table_t *label_table, label_t input);

/**
 * Looks up and returns a pointer to a matching label as with
 * the name provided in the arguments list, obtained within the
 * label table provided.
 *
 * @param label_table The label table to search within.
 * @param name The name of the label to search for.
 *
 * @return Pointer to the label if found, NULL otherwise.
 */
label_t *label_lookup(label_table_t *label_table, char *name);

typedef struct label_table {
  label_t *entries; /* Pointer to array of labels as part of table*/
  size_t size;      /* Total size offset of the label table */
} label_table_t;

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