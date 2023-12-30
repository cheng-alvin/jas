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
 * @note Not to be confused with `jasInstructionEncoderPointerArray_t`.
 * @see `jasInstructionEncoderPointerArray_t` for more information.
 * (below this function type definition)
 */

typedef jasErrorCode_t (*jasInstructionEncoderPointer_t)(jasTaggedOperand_t, jasTaggedOperand_t, jasTaggedOperand_t, jasTaggedOperand_t, jasInstance_t *);

// Array version of the function pointer. (basically just the function pointer duplicated)
typedef jasInstructionEncoderPointer_t jasInstructionEncoderPointerArray_t[];

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

jasErrorCode_t codegen(jasInstruction_t instruction, jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance);

#endif