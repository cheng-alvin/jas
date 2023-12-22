#ifndef JAS_REX_H
#define JAS_REX_H

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Enum for defining the REX prefix modes bits, which are
 * used to extend the typical instruction range and other
 * operand properties.
 *
 * Each enum value sets a letter indicated flag inside the
 * REX prefix byte. The REX prefix byte is defined as follows:
 *
 *   7                           0
 * +---+---+---+---+---+---+---+---+
 * | 0   1   0   0 | W | R | X | B |
 * +---+---+---+---+---+---+---+---+
 *
 * As shown, the first 4 bits are always 0100 to specify the
 * REX prefix, and the corresponding bit would be set to 1 depending
 * on the enum value.
 *
 * @see https://wiki.osdev.org/X86-64_Instruction_Encoding#REX_prefix
 *
 */
typedef enum {
  JAS_REX_W = 0b00001000,
  JAS_REX_R = 0b00000100,
  JAS_REX_X = 0b00000010,
  JAS_REX_B = 0b00000001,
} jasRexPrefix_t;

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * A silly helper that constructs a REX prefix byte with the given
 * prefix type with an enum. If the existing byte is `NULL`, then the
 * `REX_BASE` byte is used instead, and a new REX prefix byte is returned.
 *
 * @param existingByte The existing byte to append the prefix to.
 * @param prefixType The prefix type to append to the existing byte.
 *
 * @returns The REX prefix byte with the given prefix type or the
 * one that is appended by the `prefixType` parameter.
 */

uint8_t jasRexConstructPrefix(uint8_t existingByte, jasRexPrefix_t prefixType)
#endif