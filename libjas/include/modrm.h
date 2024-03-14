#ifndef MODRM_H
#define MODRM_H

#include <stdint.h>

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Enumeration used for identifying the different types of operands
 * and addressing modes used in the x86 processors' ModR/M byte.
 * This is used to pass items into the `jasGenerateModrm()` function,
 * it also indicates for what to write into the buffer.
 */

typedef enum {

  /**
   * @brief
   * This mode is used to indicate that the operand is an indirect
   * memory address. This means that the operand is a memory address
   * that is stored in a register.
   *
   * @example mov eax, [ebx]
   */

  JAS_MODRM_INDIRECT = 0b00000000,

  /**
   * @brief
   * This mode is used to indicate that the operand is an indirect
   * memory address with a following 8-bit displacement. This means
   * that the operand is a memory address that is stored in a register
   * with an 8-bit displacement.
   *
   * @example mov eax, [ebx + 0x10]
   * (The displacement would be 0x10 and would be dumped into the buffer)
   */

  JAS_MODRM_FOLLOWING_DISPLACEMENT_8 = 0b01000000,

  /**
   * @brief
   * This mode is used to indicate that the operand is an indirect
   * memory address with a following 64-bit displacement. This means
   * that the operand is a memory address that is stored in a register
   * with a 64-bit displacement.
   *
   * @example mov eax, [rbx + 0x100000000000000]
   * (The displacement would be 0x10000000 and would be dumped into the buffer)
   */

  JAS_MODRM_FOLLOWING_DISPLACEMENT_64 = 0b10000000,

  /**
   * @brief
   * This mode is used to indicate that the operand is a register. This
   * means that the operand is a register and not a memory address.
   *
   * @example mov eax, ebx
   */

  JAS_MODRM_REGISTER = 0b11000000,
} jasModrmMode_t;

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * This function is used to generate the ModR/M byte for the x86
 * processor family. It takes in the `mod`, `reg`, and `rm` parameters
 * and concatenates them into a single byte. This byte is then used
 * to write into the buffer.
 *
 * @note Please see the internal documentation for more information
 * on how the magic numbers work.
 * @see `write8()` for how to write bytes into a buffer for Jas.
 *
 * @param mod The ModR/M byte's mode.
 * @param reg The ModR/M byte's register bits.
 * @param rm The ModR/M byte's RM bits.
 *
 * @note Since 2 + 3 + 3 = 8. Hence, The `reg` and `rm` parameters
 * shall be variables with only 3 bits in length. And the `mod` parameter
 * shall be a variable with only 2 bits wide. This is to ensure that the
 * ModR/M byte is only 1 byte in length.
 *
 * @example You can use `mod & 0b11` or `reg/rm & 0b111` to limit the
 * width of the variables inside your code.
 *
 * @return The processed ModR/M byte from the parameters.
 */

uint8_t jasGenerateModrm(jasModrmMode_t mod, uint8_t reg, uint8_t rm);

#endif
