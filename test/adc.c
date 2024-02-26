#include "codegen.h"
#include "init.h"
#include "labrador/labrador.h"
#include <stdlib.h>

TEST() {
  jasInstance_t tempRealModeInstance;
  jasInstance_t realModeInstance;
  jasInstance_t longModeInstance;
  jasInitNew(JAS_MODE_16, &realModeInstance, JAS_NULL, JAS_NULL);
  jasInitNew(JAS_MODE_16, &tempRealModeInstance, JAS_NULL, JAS_NULL);
  jasInitNew(JAS_MODE_64, &longModeInstance, JAS_NULL, JAS_NULL);

  ({
    const jasReg8_t reg8 = JAS_REG_AL;
    const jasReg16_t reg16 = JAS_REG_AX;
    const jasReg32_t reg32 = JAS_REG_EAX;
    const jasReg64_t reg64 = JAS_REG_RAX;

    const uint8_t val8 = 0xFF;
    const uint16_t val16 = 0xFFFF;
    const uint32_t val32 = 0xFFFFFFFF;

    const jasTaggedOperand_t al = jasConstructOperand(&reg8, JAS_REG_OPERAND_8);
    const jasTaggedOperand_t ax = jasConstructOperand(&reg16, JAS_REG_OPERAND_16);
    const jasTaggedOperand_t eax = jasConstructOperand(&reg32, JAS_REG_OPERAND_32);
    const jasTaggedOperand_t rax = jasConstructOperand(&reg64, JAS_REG_OPERAND_64);

    const jasTaggedOperand_t imm8 = jasConstructOperand(&val8, JAS_OPERAND_8);
    const jasTaggedOperand_t imm16 = jasConstructOperand(&val16, JAS_OPERAND_16);
    const jasTaggedOperand_t imm32 = jasConstructOperand(&val32, JAS_OPERAND_32);

    const jasErrorCode_t status = jasCodegen(ADC, al, imm8, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[0], 0x14);
    SHOULD_EQUAL(realModeInstance.buffer[1], 0xFF);

    const jasErrorCode_t status2 = jasCodegen(ADC, ax, imm16, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status2, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[2], 0x66);
    SHOULD_EQUAL(realModeInstance.buffer[3], 0x15);
    SHOULD_EQUAL(realModeInstance.buffer[4], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[5], 0xFF);

    const jasErrorCode_t status3 = jasCodegen(ADC, eax, imm32, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status3, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[6], 0x15);
    SHOULD_EQUAL(realModeInstance.buffer[7], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[8], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[9], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[10], 0xFF);

    const jasErrorCode_t status4 = jasCodegen(ADC, rax, imm32, JAS_NO_OPERAND, JAS_NO_OPERAND, &tempRealModeInstance);

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
    const jasReg8_t reg8 = JAS_REG_CL;
    const jasReg16_t reg16 = JAS_REG_BX;
    const jasReg32_t reg32 = JAS_REG_EBX;
    // const jasReg64_t reg64 = JAS_REG_RAX;

    const uint8_t val8 = 0xFF;
    const uint16_t val16 = 0xFFFF;
    const uint32_t val32 = 0xFFFFFFFF;

    const jasTaggedOperand_t cl = jasConstructOperand(&reg8, JAS_REG_OPERAND_8);
    const jasTaggedOperand_t bx = jasConstructOperand(&reg16, JAS_REG_OPERAND_16);
    const jasTaggedOperand_t ebx = jasConstructOperand(&reg32, JAS_REG_OPERAND_32);
    // const jasTaggedOperand_t rax = jasConstructOperand(&reg64, JAS_REG_OPERAND_64);

    const jasTaggedOperand_t imm8 = jasConstructOperand(&val8, JAS_OPERAND_8);
    const jasTaggedOperand_t imm16 = jasConstructOperand(&val16, JAS_OPERAND_16);
    const jasTaggedOperand_t imm32 = jasConstructOperand(&val32, JAS_OPERAND_32);

    const jasErrorCode_t status = jasCodegen(ADC, cl, imm8, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[11], 0x80);
    SHOULD_EQUAL(realModeInstance.buffer[12], 0xD1);
    SHOULD_EQUAL(realModeInstance.buffer[13], 0xFF);

    const jasErrorCode_t status2 = jasCodegen(ADC, bx, imm16, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status2, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[14], 0x66);
    SHOULD_EQUAL(realModeInstance.buffer[15], 0x81);
    SHOULD_EQUAL(realModeInstance.buffer[16], 0xD3);
    SHOULD_EQUAL(realModeInstance.buffer[17], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[18], 0xFF);

    const jasErrorCode_t status3 = jasCodegen(ADC, ebx, imm32, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status3, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[19], 0x81);
    SHOULD_EQUAL(realModeInstance.buffer[20], 0xD3);
    SHOULD_EQUAL(realModeInstance.buffer[21], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[22], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[23], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[24], 0xFF);
  });

  free(realModeInstance.buffer);
  free(longModeInstance.buffer);
  free(tempRealModeInstance.buffer);

  PASS
}