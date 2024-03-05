#ifndef OPERAND_H
#define OPERAND_H

#include "error.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Enum for representing the operand type. This is used to store the
 * operand types. Since C unions cannot be identified by their type,
 * we need to use the idea of tagging a union along with another struct
 * element. This is used to store the operand type and contents which
 * can be declared within different sizes and data widths as mentioned
 * below.
 *
 * @note I recommend reading the software guides of x86 just to get down
 * the basics of the assembly language and it's encoding.
 *
 * @see Intel® 64 and IA-32 Architecture
 */

typedef enum {
  JAS_NO_OPERAND_TYPE,
  JAS_REG_OPERAND_8,
  JAS_REG_OPERAND_16,
  JAS_REG_OPERAND_32,
  JAS_REG_OPERAND_64,
  JAS_OPERAND_8,
  JAS_OPERAND_16,
  JAS_OPERAND_32,
  JAS_OPERAND_64,
  JAS_INDIRECT_8,
  JAS_INDIRECT_16,
  JAS_INDIRECT_32,
  JAS_INDIRECT_64,
  JAS_REG_OPERAND_8_DISP,
  JAS_REG_OPERAND_16_DISP,
  JAS_REG_OPERAND_32_DISP,
  JAS_REG_OPERAND_64_DISP,
} jasOperandType_t;

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
  JAS_REG_CL,
  JAS_REG_DL,
  JAS_REG_BL,
  JAS_REG_AH,
  JAS_REG_CH,
  JAS_REG_DH,
  JAS_REG_BH,
  JAS_REG_SPL,
  JAS_REG_BPL,
  JAS_REG_SIL,
  JAS_REG_DIL,
  JAS_REG_R8B,
  JAS_REG_R9B,
  JAS_REG_R10B,
  JAS_REG_R11B,
  JAS_REG_R12B,
  JAS_REG_R13B,
  JAS_REG_R14B,
  JAS_REG_R15B,
} jasReg8_t;

typedef enum {
  JAS_REG_AX,
  JAS_REG_CX,
  JAS_REG_DX,
  JAS_REG_BX,
  JAS_REG_SP,
  JAS_REG_BP,
  JAS_REG_SI,
  JAS_REG_DI,
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
  JAS_REG_ECX,
  JAS_REG_EDX,
  JAS_REG_EBX,
  JAS_REG_ESP,
  JAS_REG_EBP,
  JAS_REG_ESI,
  JAS_REG_EDI,
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
  JAS_REG_RCX,
  JAS_REG_RDX,
  JAS_REG_RBX,
  JAS_REG_RSP,
  JAS_REG_RBP,
  JAS_REG_RSI,
  JAS_REG_RDI,
  JAS_REG_R8,
  JAS_REG_R9,
  JAS_REG_R10,
  JAS_REG_R11,
  JAS_REG_R12,
  JAS_REG_R13,
  JAS_REG_R14,
  JAS_REG_R15
} jasReg64_t;

// Tiny helper macro for expressing a blank operand.
#define JAS_NO_OPERAND ((jasTaggedOperand_t){.type = JAS_NO_OPERAND_TYPE})

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Struct for representing the register with displacement. This is
 * used to store the register with displacement. And can be used
 * when you need syntax like: `[rax + 4]` or `[rax + rbx]`.
 *
 * This following structs will be stored within the union below
 * for memory efficiency. Please see the operand types above this
 * post.
 */

typedef struct {
  jasReg8_t reg8;
  uint8_t displacement;
} jasRegDisplacement8_t;

typedef struct {
  jasReg16_t reg16;
  uint16_t displacement;
} jasRegDisplacement16_t;

typedef struct {
  jasReg32_t reg32;
  uint32_t displacement;
} jasRegDisplacement32_t;

typedef struct {
  jasReg64_t reg64;
  uint64_t displacement;
} jasRegDisplacement64_t;

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
 * @see `jasRegDisplacement8_t`, `jasRegDisplacement16_t`,
 * `jasRegDisplacement32_t`, `jasRegDisplacement64` for more
 * information on the registers with displacement addresses.
 *
 * @see AMD64 Architecture Programmer’s Manual 1 - 1 3.1.2
 * @see Intel® 64 and IA-32 Architectures 1 - 3.7.2.1
 */

typedef union {
  jasReg16_t reg16;
  jasReg32_t reg32;
  jasReg64_t reg64;
  jasReg8_t reg8;
  jasReg16_t indirectReg16;
  jasReg32_t indirectReg32;
  jasReg64_t indirectReg64;
  jasReg8_t indirectReg8;
  jasRegDisplacement8_t regDisp8;
  jasRegDisplacement16_t regDisp16;
  jasRegDisplacement32_t regDisp32;
  jasRegDisplacement64_t regDisp64;
} jasRegOperandType_t;

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
 *
 * @note Is untagged because it is used to store the operand type
 * and contents which can be declared within different sizes and
 * data widths. NOT for use in functions.
 */

typedef union {
  uint8_t operand8;
  uint16_t operand16;
  uint32_t operand32;
  uint64_t operand64;
  jasRegOperandType_t reg;
} jasUntaggedOperand_t;

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Struct for representing the tagged operand. This is used to store
 * the tagged operand using the `jasOperandType_t` enum. This allows
 * untagged generic unions to be identified by their type. Since
 * C does not provide a way to index them.
 */

typedef struct {
  jasOperandType_t type;
  jasUntaggedOperand_t operand;
} jasTaggedOperand_t;

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Helper function for constructing the tagged operand. This is used
 * to construct the tagged operand using the `jasOperandType_t` enum,
 * without having messy syntax and inlines throughout.
 *
 * @param value The value to be stored within the operand.
 * @param type The type of the operand.
 *
 * @returns The tagged operand.
 */

jasTaggedOperand_t jasConstructOperand(void *value, jasOperandType_t type);

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Helper to check if the operand is a register that requires the REX
 * prefix byte. The REX prefix byte allows override of the registers
 * allowing for more options such as the r8b-r15b registers.
 *
 * @param x The tagged operand to be checked.
 *
 * @returns Whether the operand is a register that requires the REX
 * prefix in a boolean form.
 *
 * @note Once again, I recommend reading the software guides of x86
 * just to get down the basics of the assembly language and it's
 * encoding and what REX prefixes are.
 */

bool jasRexExpectedInRegisterEncoding(jasTaggedOperand_t x);

// TODO Docs.
uint8_t jasGetRegField(uint8_t x);

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Since intel instructions DO NOT support using the high registers
 * while another register is being used with a REX prefix, this
 * function is used for checking if the higher registers are encodable.
 *
 * @see Intel® 64 and IA-32 Architectures (Volume 2A) - 3.1.1.1
 *
 * (As usual, if you are interested in writing assembler code, please
 * go ahead and read the Intel manuals.)
 *
 * @param op1 The first operand to be checked.
 * @param op2 The second operand to be checked.
 * @returns The error code if the high registers are invalid or not
 */

bool jasCheckIfHighRegistersAreValidUnderRexPrefix(jasTaggedOperand_t op1, jasTaggedOperand_t op2);

#endif
