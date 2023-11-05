#include "constants.h"
#include <stdint.h>

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * This function writes the `AAA` instruction to the specified buffer
 * inside the first parameter. The `AAA` instruction is used to adjust
 * the result of a previous `DAA` instruction.
 *
 * @note Please see the intel manual for more information on the instruction.
 * @note Throws error if mode is specified to: `LONG`.
 *
 * @param buffer The buffer to write into.
 * @param mode The x86 operating mode for the assembly code.
 */

uint8_t aaa(uint8_t *buffer, modes_t mode);
