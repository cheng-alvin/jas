#include <stdint.h>

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * This function writes the `AAS` instruction to the specified buffer
 * inside the hints. The `AAS` instruction is used to adjust
 * the result of a previous `DAS` instruction.`
 *
 * @note Also returns `NULL` if errors occur.
 * @note Please see the intel manual for more information on the instruction.
 *
 * @returns A pointer to the buffer address.
 */

uint8_t *aas();