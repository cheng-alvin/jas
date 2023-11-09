#include "debug_info.h"
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
 * @param info The debug information for the instruction.
 *
 * @returns `NULL` on error.
 * @returns The pointer to the buffer on success.
 */

uint8_t *aaa(uint8_t *buffer, info_t *info);
