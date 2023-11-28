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