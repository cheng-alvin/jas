#ifndef JAS_INSTRUCTION_H
#define JAS_INSTRUCTION_H

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Enum for defining the several instructions that are
 * present in the x86 architecture. Also allows functions
 * and `switch` statements to use for easier readability
 * and access.
 */

typedef enum {
  AAA,
  AAD,
} instruction_t;

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Converts a string instruction into an `instruction_t` enum.
 * useful for converting a string instruction into an enum and
 * allowing for using `switch` statement multiple times in the
 * code. (Basically allows for a faster speed).
 *
 * @see `instruction_t`
 *
 * @param instruction The instruction to convert to an enum.
 * @returns The enum of the instruction.
 */

instruction_t jasToInstructionEnum(char *instruction);

#endif