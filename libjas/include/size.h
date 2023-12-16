#ifndef JAS_SIZE_H
#define JAS_SIZE_H

#include "error.h"
#include "null.h"
#include "operand.h"
#include <stdint.h>

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Finds the size of an operand. Just simply searches the operand
 * struct for information and checking if it's `NULL`.
 *
 * @note Not used for finding the size of a register, Please use
 * `jasRegisterSize` instead.
 *
 * @param x The operand to find the size of.
 * @returns The size of the operand.
 */

int8_t jasOperandSize(jasOperand_t *x);

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Finds the size of a register operand. Just simply searches the operand
 * struct for information and checking if it's `NULL`.
 *
 * @note Not used for finding the size of an immediate. Please use
 * `jasOperandSize` instead.
 *
 * @param x The operand to find the size of.
 * @returns The size of the register operand.
 */

int8_t jasRegisterSize(jasOperand_t *x);

#endif