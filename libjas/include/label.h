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

#ifndef LABEL_H
#define LABEL_H

#include <stdbool.h>
#include <stddef.h>

/**

 * Function for creating a new label with the given name and address
 * as mentioned in the `label_t` struct, storing it in an internal
 * table as a label entry.
 *
 * @param name The name of the label.
 * @param exported Boolean for whether the label is exported or not.
 * @param ext Boolean for whether the label is external or not.
 * @param address The address of the label.
 * @param instr_index The index to slip the label in the instruction array.
 *
 * @note EITHER `instr_index` or `address` fields can be set as `NULL`, the
 * assembler will infer the `address` field from the `instr_index` field if
 * no explict `address` field is provided, however, if an explicit `address`
 * field is provided, the `instr_index` field will be ignored.
 *
 * A `
 * @note The address depicts a position in the buffer, and can be
 * obtained using `buf.len` after encoding the instruction from
 * the `buffer.h`, or many times the `buf` variable. Therefore,
 * allowing the relative addressing of the label to be calculated
 * on the fly.
 */
void label_create(char *name, bool exported, bool ext, size_t address, size_t instr_index);

typedef struct {
  char *name;         /* Name of the label in a string format */
  bool exported;      /* Boolean for whether the label is exported to the linker table or not */
  bool ext;           /* Boolean for whether the label is external or not (If a relocation table should be created) */
  size_t address;     /* Address of the label entry, can use `buf.len` */
  size_t instr_index; /* Index to slip the label in the instruction array */
} label_t;

extern label_t *label_table;
extern size_t label_table_size;

/**
 * Function for destroying the label table, freeing the memory
 * allocated for the label names and the label entries itself.
 *
 * @warning This function will free all the memory allocated for
 * the label entries and the label names, and will set the label
 * table to `NULL` and the label table size to `0`. Potentially
 * destructive!
 */
void label_destroy_all();

/**
 * Function for looking up a label in the label table, and retu-
 * rning the label entry if found, otherwise returning a `NULL`
 * label entry will be returned back to the caller.
 *
 * @param name The name of the label to look up in the label table.
 *
 * @note Caller is responsible for freeing the memory allocated
 * and handling string creations and declarations.
 * '
 * @return The pointer to the label entry if found, otherwise `NULL`
 */
label_t *label_lookup(char *name);

#endif
