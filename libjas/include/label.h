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
  LABEL_NULL = 0,
  LABEL_LOCAL,
  LABEL_GLOBAL,
  LABEL_EXTERN, // Associated with external linkage, TBD
};

/**
 * Function used for the generation of a complete label table
 * based off an array of encoded/serialized instructions, and
 * evaluating the label's addresses and callers.
 *
 * @param instr The array of encoded instructions.
 * @param generics The array of instruction/directive generics.
 * @param s The size of the instruction generic array.
 *
 * @return The generated label table.
 *
 * @note The array of serialized instructions must be padded
 * with zeroed-out entries for non-instruction generics such as
 * program or label directives.
 */

label_table_t label_table_gen(
    enc_serialized_instr_t *instr, // Should be padded!
    instr_generic_t *generics, size_t s);

typedef struct label {
  /// @note Should be allocated in accordance to the `count` variable
  /// and should not be confused with `buffer_t` as it only carries
  /// an array of single-byte data with an associated size.

  struct {
    uint64_t *offsets; /* List of callers of callers' offsets */
    size_t count;      /* Number of callers */
  } callers;

  char *name;     /* Name of the label in a string format */
  size_t address; /* Offset of label's definition relative to `.text` */

  enum label_type type; /* Type of the label */

  /// implementation @note The relative offset may be calculated
  /// during the encoding phase, through the use of `address`
  /// subtracted by the current caller's instruction pointer.
} label_t;

/// @see `label_free()`:

#define label_free_table(table)             \
  do {                                      \
    for (size_t i = 0; i < table.size; i++) \
      label_free(&table.entries[i]);        \
    table.size = 0;                         \
  } while (0)

/**
 * Frees allocated memory within an allocated label structure
 * pointer. This function assumes the structure itself passed
 * into the function is allocated dynamically.
 *
 * @param label Pointer of the label to be deallocated
 *
 * @note To automatically free the entire label table, use
 * the shown `label_free_table` macro instead that combines
 * function calls.
 */
void label_free(label_t *label);

/**
 * Function for creating a new label entry in the label table,
 * handling required memory allocations and duplication checks.
 * However, this function does not exempt the caller's responsibility
 * of freeing the buffer after use.
 *
 * @param label_table The label table to store the label entry in.
 * @param input New table entry to be added to the label table.
 *
 * @return The success the of the label creation is indicated back
 * to the caller. A non-zero value indicates failure.
 */

uint8_t label_create(label_table_t *label_table, label_t input);

// Macros that simplify label type checks, akin to the `op_` macros
// that can be used with the `operand_t` structure and type checks.
// Such macros can be implemented in `if` statements for improved
// readability and consistency across the codebase.

#define label_exported(x) ((x.type) == LABEL_GLOBAL)
#define label_extern(x) ((x.type) == LABEL_EXTERN)

#define label_null(x) ((x.type) == LABEL_NULL)

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
 * Function for facilitating the automatic computation of label
 * relative offsets, across all operation modes. `label_evaluate`
 * is designed purely for delegation between a label address and
 * current address and serialized instructions.
 *
 * @param instr The serialized structure that lacks a label offset
 * @param current The current address, till end of this instruction.
 * @param label Address in offset of the label in question.
 * @param mode Current operating mode for the encoder's target.
 *
 * @return A pointer of the `instr` and indicative of success,
 * or of an error, where `NULL` is returned.
 */
enc_serialized_instr_t *label_evaluate(
    enc_serialized_instr_t *instr,
    uint64_t current, uint64_t label, enum modes mode);

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