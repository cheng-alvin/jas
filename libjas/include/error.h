
#ifndef JAS_ERROR_H
#define JAS_ERROR_H

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Enum that specifies the error codes when using the assembler.
 * This is used to indicate the status of the assembler and its
 * functions. This is also used to indicate the status of the
 * utility functions and what not. (i.e Assembly syntax error or
 * input error).
 */

typedef enum {
  JAS_NO_ERROR,
  JAS_UNDEFINED_POINTER,
  JAS_INVALID_INSTRUCTION,
  JAS_NON_LONG_MODE_INSTRUCTION,
  JAS_OPERAND_ERROR,
  JAS_MODE_ERROR,
  JAS_INVALID_HIGH_REGISTER,
  JAS_LONG_MODE_INSTRUCTION
} jasErrorCode_t;

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Function used to get the error code's string representation.
 * This is used to get the error code's string formatted response
 * for printing to the user.
 *
 * @param c The error code to get the string representation of.
 *
 * @returns The string representation of the error code.
 * @see `jasErrorCode_t` for more information on the error codes.
 */

char *jasErrorno(jasErrorCode_t c);

#endif
