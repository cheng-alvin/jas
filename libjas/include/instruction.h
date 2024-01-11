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
  AAM,
  AAS,
  ADC,
  ADD,
  AND,
  BOUND,
  BSF,
  BSR,
  BSWAP,
  BT,
  BTC,
  BTR,
  BTS,
  CALL,
  CBW,
  CDQE,
  CWDE,
} jasInstruction_t;

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Converts a string instruction into an `jasInstruction_t` enum.
 * useful for converting a string instruction into an enum and
 * allowing for using `switch` statement multiple times in the
 * code. (Basically allows for a faster speed).
 *
 * @see `jasInstruction_t`
 *
 * @param instruction The instruction to convert to an enum.
 * @returns The enum of the instruction.
 *
 * @note Returns `-1` if the opcode is not found.
 */

jasInstruction_t jasToInstructionEnum(char *instruction);

#endif
