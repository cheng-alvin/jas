/**
 * mit license
 * copyright (c) 2023-2025 alvin cheng <eventide1029@gmail.com>
 *
 * permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "software"), to deal
 * in the software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the software, and to permit persons to whom the software is
 * furnished to do so, subject to the following conditions:
 *
 * the above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the software.
 *
 * the software is provided "as is", without warranty of any kind, express or
 * implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose and noninfringement. in no event shall the
 * authors or copyright holders be liable for any claim, damages or other
 * liability, whether in an action of contract, tort or otherwise, arising from,
 * out of or in connection with the software or the use or other dealings in the
 * software.
 *
 * @see `license`
 */

#ifndef REX_H
#define REX_H

#include "mode.h"
#include "operand.h"
#include <stdint.h>

/**
 * Type alias for a single byte representing the REX byte value
 * which is prepended to the beginning of the instruction in order
 * to alter certain properties of its operands, as described by
 * the Intel Systems Programming Guide. Conventions for its use
 * appear below:
 *
 * A variable carrying the REX prefix for appending to an instruction
 * should be initalized with the `REX_DEFAULT` value. The value
 * represents an absent REX prefix and can be filtered out.
 *
 * Subsequent additions modifications to the prefix can be done
 * through the manual combination of provided values via bitwise
 * operations such as `|`.
 */
typedef uint8_t rex_t;

// Constants for the REX prefix bits and corresponding meanings,
// as documented below from the Intel Systems Programming Guide:

#define REX_W 0x48 // Allows 64-bit operand sizes
#define REX_R 0x44 // Extension of ModR/M reg field
#define REX_X 0x42 // Extension of SIB index field
#define REX_B 0x41 // Extends ModR/M r/m, SIB base, or opcode

#define REX_DEFAULT 0b01000000

/// @note forward declaration to prevent circular dependency.
typedef struct instruction instruction_t;

/**
 * Function for applying the REX prefix to the instruction based
 * on the instruction's operands and the current operating mode,
 * returning the final resulting REX byte to be written into
 * the instruction.
 *
 * @param input The instruction generic pointer
 * @return The final resulting REX prefix byte.
 */
rex_t rex_apply(instruction_t *input);

#endif
