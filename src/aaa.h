#include <stdint.h>

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * This function writes the `AAA` instruction to the specified buffer
 * inside the hints. The `AAA` instruction is used to adjust
 * the result of a previous `DAA` instruction.
 *
 * @note Please see the intel manual for more information on the instruction.
 * @note Also returns `NULL` if errors occur.
 *
 * @returns A pointer to the buffer address.
 */

uint8_t *aaa();