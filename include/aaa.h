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
 *
 * @param buffer The buffer to write into.
 * @returns The pointer to the buffer on success.
 */

uint8_t *aaa(uint8_t *buffer);