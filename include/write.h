#include <stdint.h>

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Writes 8 bits or a single byte to the specified `buffer`
 * pointer. Useful for generating machine code in executables.
 *
 * @param buffer The pointer for the buffer to write to.
 * @param value The value to write.
 *
 * @note Please note that the value should be 1 byte
 * or 8 bits wide when passing into this function.
 *
 * @returns The pointer to the buffer.
 */

uint8_t *write8(uint8_t *buffer, uint8_t value);
