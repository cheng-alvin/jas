#include "codegen.h"
#include "init.h"
#include "labrador/labrador.h"
#include <stdlib.h>

TEST() {
  ({
    jasInstance_t realModeInstance;
    jasInstance_t longModeInstance;
    jasInitNew(JAS_MODE_16, &realModeInstance, JAS_NULL, JAS_NULL);
    jasInitNew(JAS_MODE_64, &longModeInstance, JAS_NULL, JAS_NULL);

    const uint8_t contentArr[] = {JAS_REG_AL, JAS_REG_AX, JAS_REG_EAX, JAS_REG_RAX, 0xFF, 0xFFFF, 0xFFFFFFFF};
    const jasTaggedOperand_t al = jasConstructOperand(&contentArr[0], JAS_REG_OPERAND_8);
    const jasTaggedOperand_t ax = jasConstructOperand(&contentArr[1], JAS_REG_OPERAND_16);
    const jasTaggedOperand_t eax = jasConstructOperand(&contentArr[2], JAS_REG_OPERAND_32);
    const jasTaggedOperand_t rax = jasConstructOperand(&contentArr[3], JAS_REG_OPERAND_64);

    const jasTaggedOperand_t imm8 = jasConstructOperand(&contentArr[4], JAS_OPERAND_8);
    const jasTaggedOperand_t imm16 = jasConstructOperand(&contentArr[5], JAS_OPERAND_16);
    const jasTaggedOperand_t imm32 = jasConstructOperand(&contentArr[6], JAS_OPERAND_32);

    const jasErrorCode_t status = jasCodegen(ADC, al, imm8, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[0], 0x14);
    SHOULD_EQUAL(realModeInstance.buffer[1], 0xFF);

    const jasErrorCode_t status2 = jasCodegen(ADC, ax, imm16, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status2, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[2], 0x15);
    SHOULD_EQUAL(realModeInstance.buffer[3], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[4], 0xFF);

    const jasErrorCode_t status3 = jasCodegen(ADC, eax, imm32, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status3, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[5], 0x15);
    SHOULD_EQUAL(realModeInstance.buffer[6], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[7], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[8], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[9], 0xFF);

    const jasErrorCode_t status4 = jasCodegen(ADC, rax, imm32, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status4, JAS_LONG_MODE_INSTRUCTION);

    const jasErrorCode_t status5 = jasCodegen(ADC, rax, imm32, JAS_NO_OPERAND, JAS_NO_OPERAND, &longModeInstance);

    SHOULD_EQUAL(status5, JAS_NO_ERROR);
    SHOULD_EQUAL(longModeInstance.buffer[0], 0x48);
    SHOULD_EQUAL(longModeInstance.buffer[1], 0x15);
    SHOULD_EQUAL(longModeInstance.buffer[2], 0xFF);
    SHOULD_EQUAL(longModeInstance.buffer[3], 0xFF);
    SHOULD_EQUAL(longModeInstance.buffer[4], 0xFF);
    SHOULD_EQUAL(longModeInstance.buffer[5], 0xFF);
  });

  ({

  });

  PASS
}