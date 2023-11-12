#include "null.h"
#include <stdint.h>

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
 * The info_t struct is used to store the information of the
 * assembler module, including the program mode and the buffer
 * that stores the generated assembler code.
 *
 * @see `mode_t` for more information about the program mode
 * enum.
 *
 * TODO Revise docs for struct?
 */

typedef struct {
  mode_t program_mode;
  uint8_t *buffer;
} info_t;

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * The variable declaration used to store the information of
 * the assembler module.
 */

extern info_t __jas_info__;