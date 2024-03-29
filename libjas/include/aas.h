#ifndef AAS_H
#define AAS_H

#include "error.h"

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Function for Generating the code for the `aas` instruction, normally
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
 * @note All parameters are not required. Please see the intel manual for more
 * information on the `aas` instruction.
 *
 * @note This instruction is NOT allowed to be encoded in 64-bit of long mode.
 * (Which throws an error if the mode is set to 64-bit)
 */

jasErrorCode_t jasAAS(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance);

#endif