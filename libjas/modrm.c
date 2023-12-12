#include "modrm.h"
#include <stdint.h>

// TODO Convert to macro?!?
// TODO Rename?

uint8_t jasGenerateModRM(jasModRMMode_t mod, uint8_t reg, uint8_t rm) {
  /**
   * Uses binary ORs to concatenate the `mod`, `reg`, and
   * `rm` parameter byte values into a single byte. After this is
   * done, it can be used to written onto the array using the
   * `write8()` function.
   *
   * @see write.c
   *
   * * Firstly, the `mod` parameter is left in memory.
   * * Secondly, the `reg` parameter is shifted left by 3 bits.
   * (This is done to pad the lower bits with 0s and makes sure the
   * `rm` parameter is not overwritten by the `reg` parameter).
   * * Thirdly, the `rm` parameter is left in memory and OR-ed together.
   *
   * @example
   *
   * mod            = 0b11000000
   * ( reg          = 0b00000111 )
   * (reg << 3)     = 0b00111000
   * rm             = 0b00000001
   * ----------------------------
   * result         = 0b11111001
   *
   * So, that's hot these magic numbers work:
   */

  return mod | (reg << 3) | rm;
}

// Honestly, I don't know what this does.
// Lets just wing' it.
// Basically fake it until you make it! 🤣🤣🤣