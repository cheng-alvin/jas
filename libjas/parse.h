#ifndef PARSE_H
#define PARSE_H

/**
 * @deprecated
 */

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Return type for the `parse` function, specifies the instruction's
 * name and its arguments. Shall be returned from the parser function's
 * output.
 *
 * TODO Revise docs for struct?
 */

typedef struct {
  char *instruction;
  char *arg1;
  char *arg2;
  char *arg3;
} instruction_t;

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Parses assembly instructions into an array of `instruction_t` structs.
 * Assembly instructions are made of 4 parts: the instruction, and up to 3
 * operands or otherwise known as arguments. (In a traditional programming
 * sense)
 *
 * @example [instruction] [arg1], [arg2], [arg3]
 *
 * @note The instruction is space-separated from the arguments, however, the
 * arguments are comma *AND* space separated from each other.
 *
 * @param buffer The stringified assembly instructions to parse.
 *
 * @returns An array of `instruction_t` structs representing the instructions.
 */

instruction_t **parse(char *buffer);

#endif