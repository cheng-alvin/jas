#ifndef JAS_CODEGEN_H
#define JAS_CODEGEN_H

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
} jasOperand_t;

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Function pointer fo the instruction encoder. This is used to
 * encode the instruction and its operands into machine code.
 * This can be used as a placeholder for holding the instruction
 * encoder. Which is called a generation unit.
 *
 * @note This is used to store the instruction encoder function
 * in the codegen function pointer array.
 */

typedef jasErrorCode_t (*jasInstructionEncoderPointerArray_t[])(jasOperand_t, jasOperand_t, jasOperand_t, jasOperand_t, jasInstance_t *);

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Function for generating the code into raw binary code. expects
 * the instruction and operands to be passed in. This is used to
 * generate the code for the raw binary file format. Should already
 * be parse and lexed as minimal error checking is done here.
 *
 * @param instruction The instruction to be encoded.
 * @param op1 The first operand to be encoded.
 * @param op2 The second operand to be encoded.
 * @param op3 The third operand to be encoded.
 * @param op4 The fourth operand to be encoded.
 * @param instance The instance of the assembler unit.
 */

jasErrorCode_t codegen(jasInstruction_t instruction, jasOperand_t op1, jasOperand_t op2, jasOperand_t op3, jasOperand_t op4, jasInstance_t *instance);

#endif