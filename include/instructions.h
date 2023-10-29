/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * The x86 instruction set enum and opcode values used for
 * code generation as well as referencing from other locations.
 *
 * @note May not be compatible on 32 and even 16b bit systems, please see
 * external sources for more information.
 */

enum x64_opcodes {
  MOV = 0x8B,       /**< Move */
  ADD = 0x03,       /**< Add */
  SUB = 0x2B,       /**< Subtract */
  JMP = 0xE9,       /**< Jump */
  CALL = 0xE8,      /**< Call */
  RET = 0xC3,       /**< Return */
  PUSH = 0x50,      /**< Push */
  POP = 0x58,       /**< Pop */
  CMP = 0x3B,       /**< Compare */
  JZ = 0x74,        /**< Jump if Zero */
  JNZ = 0x75,       /**< Jump if Not Zero */
  JE = 0x74,        /**< Jump if Equal */
  JNE = 0x75,       /**< Jump if Not Equal */
  JL = 0x7C,        /**< Jump if Less */
  JLE = 0x7E,       /**< Jump if Less or Equal */
  JG = 0x7F,        /**< Jump if Greater */
  JGE = 0x7D,       /**< Jump if Greater or Equal */
  NOP = 0x90,       /**< No Operation */
  LEA = 0x8D,       /**< Load Effective Address */
  XOR = 0x33,       /**< Exclusive OR */
  OR = 0x0B,        /**< OR */
  AND = 0x23,       /**< AND */
  NOT = 0xF7,       /**< NOT */
  SHL = 0xC1,       /**< Shift Left */
  SHR = 0xC1,       /**< Shift Right */
  ROL = 0xC1,       /**< Rotate Left */
  ROR = 0xC1,       /**< Rotate Right */
  TEST = 0x85,      /**< Test */
  SETZ = 0x0F94,    /**< Set if Zero */
  SETNZ = 0x0F95,   /**< Set if Not Zero */
  SETE = 0x0F94,    /**< Set if Equal */
  SETNE = 0x0F95,   /**< Set if Not Equal */
  SETL = 0x0F9C,    /**< Set if Less */
  SETLE = 0x0F9E,   /**< Set if Less or Equal */
  SETG = 0x0F9F,    /**< Set if Greater */
  SETGE = 0x0F9D,   /**< Set if Greater or Equal */
  MOVZX = 0x0FB6,   /**< Move with Zero-Extend */
  MOVSX = 0x0FBE,   /**< Move with Sign-Extend */
  CDQ = 0x99,       /**< Convert Double to Quad */
  IDIV = 0xF7,      /**< Integer Divide */
  IMUL = 0xF7,      /**< Integer Multiply */
  NEG = 0xF7,       /**< Negate */
  INC = 0xFF,       /**< Increment */
  DEC = 0xFF,       /**< Decrement */
  SAL = 0xC1,       /**< Shift Arithmetic Left */
  SAR = 0xC1,       /**< Shift Arithmetic Right */
  ADC = 0x11,       /**< Add with Carry */
  SBB = 0x19,       /**< Subtract with Borrow */
  STOSB = 0xAA,     /**< Store Byte */
  LODSB = 0xAC,     /**< Load Byte */
  REP = 0xF3,       /**< Repeat */
  CLD = 0xFC,       /**< Clear Direction Flag */
  STD = 0xFD,       /**< Set Direction Flag */
  CLI = 0xFA,       /**< Clear Interrupt Flag */
  STI = 0xFB,       /**< Set Interrupt Flag */
  IN = 0xE5,        /**< Input */
  OUT = 0xE7,       /**< Output */
  HLT = 0xF4,       /**< Halt */
  INT = 0xCD,       /**< Interrupt */
  IRET = 0xCF,      /**< Interrupt Return */
  RDTSC = 0x0F31,   /**< Read Time-Stamp Counter */
  CPUID = 0x0FA2,   /**< CPU Identification */
  SYSCALL = 0x0F05, /**< System Call */
  SYSRET = 0x0F07,  /**< System Return */
  FADD = 0xD8,      /**< Floating-Point Add */
  FSUB = 0xDC,      /**< Floating-Point Subtract */
  FMUL = 0xDE,      /**< Floating-Point Multiply */
  FDIV = 0xF8,      /**< Floating-Point Divide */
  FLD = 0xC0,       /**< Floating-Point Load */
  FST = 0xD0,       /**< Floating-Point Store */
  FCOM = 0xD8,      /**< Floating-Point Compare */
  FCOMI = 0xDB,     /**< Floating-Point Compare and Set EFLAGS */
  FCOMIP = 0xDF,    /**< Floating-Point Compare and Set EFLAGS and Pop */
  FUCOM = 0xDD,     /**< Floating-Point Unordered Compare */
  FUCOMI = 0xDB,    /**< Floating-Point Unordered Compare and Set EFLAGS */
  FUCOMIP =
      0xDF,      /**< Floating-Point Unordered Compare and Set EFLAGS and Pop */
  FLDZ = 0xD9,   /**< Floating-Point Load Zero */
  FLD1 = 0xD9,   /**< Floating-Point Load One */
  FLDPI = 0xDB,  /**< Floating-Point Load PI */
  FLDLN2 = 0xDD, /**< Floating-Point Load Log2(e) */
  FLDL2E = 0xD9, /**< Floating-Point Load Log2(e) */
  FLDL2T = 0xD9, /**< Floating-Point Load Log2(10) */
  FLDLG2 = 0xDD, /**< Floating-Point Load Log10(2) */
  FLDLN2T = 0xD9, /**< Floating-Point Load Log2(e) */
  FLDLG2T = 0xD9, /**< Floating-Point Load Log10(2) */
  FSTP1 = 0xD9,   /**< Floating-Point Store One */
  FSTP8 = 0xDD,   /**< Floating-Point Store Eight */
  FSTP9 = 0xDD    /**< Floating-Point Store Nine */
};
