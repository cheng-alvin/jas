#ifndef CLFLUSH_H
#define CLFLUSH_H

#include "error.h"
#include "operand.h"

/**
 * Function for Generating the code for the `clflush` instruction, normally
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
 * @note Only the first operand is required. Please see the intel manual for more
 * information on the `clflush` instruction.
 *
 * ? Scrap instruction?
 */

jasErrorCode_t jasCLFLUSH(jasTaggedOperand_t op1, jasTaggedOperand_t op2, jasTaggedOperand_t op3, jasTaggedOperand_t op4, jasInstance_t *instance);

#endif