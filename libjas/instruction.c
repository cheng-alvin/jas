#include "instruction.h"
#include "null.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define SUBSTITUTE_INSTRUCTION_IF_EQUATES(x, y, z) \
  if (!strcmp(x, y))                               \
    return z;

jasInstruction_t jasToInstructionEnum(char *instruction) {
  for (; *instruction; ++instruction)
    *instruction = tolower(*instruction);

  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "aaa", AAA);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "aad", AAD);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "aam", AAM);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "aas", AAS);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "adc", ADC);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "add", ADD);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "and", AND);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "arpl", ARPL);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "bound", BOUND);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "bsf", BSF);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "bsr", BSR);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "bswap", BSWAP);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "bt", BT);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "btc", BTC);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "btr", BTR);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "bts", BTS);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "call", CALL);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "cbw", CBW);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "cdqe", CDQE);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "cwde", CWDE);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "clc", CLC);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "cld", CLD);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "clflush", CLFLUSH);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "cli", CLI);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "clts", CLTS);
  SUBSTITUTE_INSTRUCTION_IF_EQUATES(instruction, "cmc", CMC)

  // TODO revise list (some instructions are missing!!)**
  return -1;
}
