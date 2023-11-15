#include <stdint.h>

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Enum for defining the type of hint to be used in the assembler
 * hinting functions. Allows the compiler to declare hint
 * information before execution.
 */

typedef enum {
  JAS_OPERATION_MODE,
  JAS_BUFFER,
} hint_t;

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Code-generation mode for the assembler module code, can
 * be used as a indicator for the assembler code generator
 * to generate code.
 */

typedef enum {
  JAS_REAL_MODE,
  JAS_PROTECTED_MODE,
  JAS_LONG_MODE,
} mode_t;

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Used to get/return the buffer and mode values from the hint
 * module (Respectively shown). Allows for quick access to the
 * "buffer" information in "OpenGL" terms.
 *
 * @returns The buffer or mode value(s).
 */
uint8_t *jasGetBuffer();
uint8_t jasGetMode();

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Used for setting a hint value to the hint module inside Jas.
 * The hints are used to program/tell the compiler on how the
 * code should be generated. For example, the `JAS_OPERATION_MODE`
 * specifies the operation mode for the code generator.
 *
 * @param type The type of hint to be used.
 * @param data The data to be used for the hint.
 *
 * @note See `mode_t` and `hint_t` for more information regarding
 * the type infomation.
 *
 */

void jasHint(hint_t type, const long data);
// TODO Make sure that the hint module has a `getHint` function lol.
