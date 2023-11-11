/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Code-generation mode for the assembler module code, can
 * be used as a indicator for the assembler code generator
 * to generate code.
 */

typedef enum {
  REAL,
  PROTECTED,
  LONG,
} mode_t;

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Generates machine code from the specified array of instructions in the
 * parsed `instruction_t` format. Used to generate machine code, but not
 * ELF files or executable files.
 *
 * @see parse.h `instruction_t` and the `parse()` will be defined in
 * that file.
 *
 * @param arr The array of instructions to generate machine code from.
 * @param len The length of the array of instructions.
 * @param mode The mode to generate the machine code in.
 * @param file The file name to generate the machine code from.
 *
 * @note DO NOT PASS THE FILE BUFFER INTO THE `file` PARAMETER,
 * INSTEAD PASS THE FILE NAME!
 *
 * @returns A pointer to the generated machine code buffer.
 */

uint8_t *codegen(instruction_t *arr, int len, mode_t mode, char *file);