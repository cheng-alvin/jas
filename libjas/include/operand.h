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
  int8_t operand8;
  int16_t operand16;
  int32_t operand32;
  int64_t operand64;
  jasRegOperandType_t reg;
} jasOperand_t;

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
  AL,
  BL,
  CL,
  DL,
  SIL,
  DIL,
  BPL,
  SPL,
  R8B,
  R9B,
  R10B,
  R11B,
  R12B,
  R13B,
  R14B,
  R15B,
  AH,
  BH,
  CH,
  DH
} jasReg8_t;

typedef enum {
  AX,
  BX,
  CX,
  DX,
  DI,
  SI,
  BP,
  SP,
  R8W,
  R9W,
  R10W,
  R11W,
  R12W,
  R13W,
  R14W,
  R15W
} jasReg16_t;

typedef enum {
  EAX,
  EBX,
  ECX,
  EDX,
  EDI,
  ESI,
  EBP,
  ESP,
  R8D,
  R9D,
  R10D,
  R11D,
  R12D,
  R13D,
  R14D,
  R15D
} jasReg32_t;

typedef enum {
  RAX,
  RBX,
  RCX,
  RDX,
  RDI,
  RSI,
  RBP,
  RSP,
  R8,
  R9,
  R10,
  R11,
  R12,
  R13,
  R14,
  R15
} jasReg64_t;