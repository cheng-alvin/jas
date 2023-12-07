#ifndef JAS_OPERAND_H
#define JAS_OPERAND_H

#include <stdint.h>

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Union for declaring the operand type and contents for the
 * assembler. This is used to store the operand type and contents
 * which can be declared within different sizes and data widths.
 *
 * @note This is used to store the operand types can be things such
 * as `char`, `int` or anything that fits within the size of the
 * union.
 */

typedef union {
  uint8_t operand8;
  uint16_t operand16;
  uint32_t operand32;
  uint64_t operand64;
  jasRegOperandType_t reg;
} jasOperand_t;

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Union for representing registers with different bit sizes. Please
 * note that the registers are represented in the union are 8/16/32/64 bit
 * registers. This allows a range to be compressed together and used.
 *
 * @see `jasReg8_t` for more information on the 8 bit registers.
 * @see `jasReg16_t` for more information on the 16 bit registers.
 * @see `jasReg32_t` for more information on the 32 bit registers.
 * @see `jasReg64_t` for more information on the 64 bit registers.
 *
 * @see AMD64 Architecture Programmer’s Manual 1 - 1 3.1.2
 * @see Intel® 64 and IA-32 Architectures 1 - 3.7.2.1
 */

typedef union {
  jasReg16_t reg16;
  jasReg32_t reg32;
  jasReg64_t reg64;
  jasReg8_t reg8;
} jasRegOperandType_t;

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Following couple enums defines the registers on different types of
 * bit modes present on the x86 processor family. More information on
 * these registers can be found on the x86 or intel programmers' docs.
 *
 * @see AMD64 Architecture Programmer’s Manual 1 - 1 3.1.2
 * @see Intel® 64 and IA-32 Architectures 1 - 3.7.2.1
 *
 * @see https://stackoverflow.com/a/51337115/15492585 for sources
 */

typedef enum {
  JAS_REG_AL,
  JAS_REG_BL,
  JAS_REG_CL,
  JAS_REG_DL,
  JAS_REG_SIL,
  JAS_REG_DIL,
  JAS_REG_BPL,
  JAS_REG_SPL,
  JAS_REG_R8B,
  JAS_REG_R9B,
  JAS_REG_R10B,
  JAS_REG_R11B,
  JAS_REG_R12B,
  JAS_REG_R13B,
  JAS_REG_R14B,
  JAS_REG_R15B,
  JAS_REG_AH,
  JAS_REG_BH,
  JAS_REG_CH,
  JAS_REG_DH
} jasReg8_t;

typedef enum {
  JAS_REG_AX,
  JAS_REG_BX,
  JAS_REG_CX,
  JAS_REG_DX,
  JAS_REG_DI,
  JAS_REG_SI,
  JAS_REG_BP,
  JAS_REG_SP,
  JAS_REG_R8W,
  JAS_REG_R9W,
  JAS_REG_R10W,
  JAS_REG_R11W,
  JAS_REG_R12W,
  JAS_REG_R13W,
  JAS_REG_R14W,
  JAS_REG_R15W
} jasReg16_t;

typedef enum {
  JAS_REG_EAX,
  JAS_REG_EBX,
  JAS_REG_ECX,
  JAS_REG_EDX,
  JAS_REG_EDI,
  JAS_REG_ESI,
  JAS_REG_EBP,
  JAS_REG_ESP,
  JAS_REG_R8D,
  JAS_REG_R9D,
  JAS_REG_R10D,
  JAS_REG_R11D,
  JAS_REG_R12D,
  JAS_REG_R13D,
  JAS_REG_R14D,
  JAS_REG_R15D
} jasReg32_t;

typedef enum {
  JAS_REG_RAX,
  JAS_REG_RBX,
  JAS_REG_RCX,
  JAS_REG_RDX,
  JAS_REG_RDI,
  JAS_REG_RSI,
  JAS_REG_RBP,
  JAS_REG_RSP,
  JAS_REG_R8,
  JAS_REG_R9,
  JAS_REG_R10,
  JAS_REG_R11,
  JAS_REG_R12,
  JAS_REG_R13,
  JAS_REG_R14,
  JAS_REG_R15
} jasReg64_t;

#endif