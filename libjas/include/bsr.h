#ifndef BSR_H
#define BSR_H

#include "error.h"

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Function for Generating the code for the `bsr` instruction, normally
 * used internally. Please call this function through the `jasCodegen()` function.
 *
 * @param op1 The first operand to be encoded.
 * @param op2 The second operand to be encoded.
 * @param op3 The third operand to be encoded.
 * @param op4 The fourth operand to be encoded.
 * @param instance The instance of the assembler unit.
 *
 * @returns The error code if the encoding was successful.
 *
 * @note The third and fourth operands are not required. Please see the intel
 * manual for more information on the `bsr` instruction.
 *
 * @warning This is NOT an assembly tutorial. Please see other resources for
 * more information on the `bsr` instruction. We expect that you know assembly and
 * basic programming concepts.
 *
 * TODO BSR bug found in bsr.c:26~31, please refer.
 */

jasErrorCode_t jasBSR(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance);

#endif