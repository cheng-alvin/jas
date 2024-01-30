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
  CLC,
  CLD,
  CLFLUSH,
  // TODO CLFLUSHOPT - Yet to be implemented
  CLI,
  CLTS,
  CMC,
  CMOVA,
  CMOVAE,
  CMOVB,
  CMOVBE,
  CMOVC,
  CMOVE,
  CMOVG,
  CMOVGE,
  CMOVL,
  CMOVLE,
  CMOVNA,
  CMOVNAE,
  CMOVNB,
  CMOVNBE,
  CMOVNC,
  CMOVNE,
  CMOVNG,
  CMOVNGE,
  CMOVNL,
  CMOVNLE,
  CMOVNO,
  CMOVNP,
  CMOVNS,
  CMOVNZ,
  CMOVO,
  CMOVP,
  CMOVPE,
  CMOVPO,
  CMOVS,
  CMOVZ,
} jasInstruction_t;

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Converts a string instruction
 * into an `jasInstruction_t` enum.
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
