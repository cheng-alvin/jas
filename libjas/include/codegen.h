#ifndef JAS_CODEGEN_H
#define JAS_CODEGEN_H

#include "error.h"
#include "init.h"
#include "instruction.h"
#include "operand.h"
#include <stdint.h>

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

typedef jasErrorCode_t (*jasInstructionEncoderPointerArray_t[])(jasUntaggedOperand_t, jasUntaggedOperand_t, jasUntaggedOperand_t, jasUntaggedOperand_t, jasInstance_t);

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

jasErrorCode_t codegen(jasInstruction_t instruction, jasUntaggedOperand_t op1, jasUntaggedOperand_t op2, jasUntaggedOperand_t op3, jasUntaggedOperand_t op4, jasInstance_t instance);

#endif