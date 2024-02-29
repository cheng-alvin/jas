#include "codegen.h"
#include "init.h"
#include "labrador/labrador.h"
#include <stdlib.h>

#define AL_IMM8 0x14
#define AX_IMM16 0x15
#define EAX_IMM32 0x15
#define RAX_IMM32 0x15

#define REG8_IMM8 0x80
#define REG16_IMM16 0x81
#define REG32_IMM32 0x81
#define REG64_IMM32 0x81

#define REG16_IMM8 0x83
#define REG32_IMM8 0x83
#define REG64_IMM8 0x83

#define REG8_REG8 0x10
#define REG16_REG16 0x11
#define REG32_REG32 0x11
#define REG64_REG64 0x11

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
    SHOULD_EQUAL(realModeInstance.buffer[0], AL_IMM8);
    SHOULD_EQUAL(realModeInstance.buffer[1], 0xFF);

    const jasErrorCode_t status2 = jasCodegen(ADC, ax, imm16, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status2, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[2], 0x66);
    SHOULD_EQUAL(realModeInstance.buffer[3], AX_IMM16);
    SHOULD_EQUAL(realModeInstance.buffer[4], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[5], 0xFF);

    const jasErrorCode_t status3 = jasCodegen(ADC, eax, imm32, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status3, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[6], EAX_IMM32);
    SHOULD_EQUAL(realModeInstance.buffer[7], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[8], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[9], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[10], 0xFF);

    const jasErrorCode_t status4 = jasCodegen(ADC, rax, imm32, JAS_NO_OPERAND, JAS_NO_OPERAND, &tempRealModeInstance);

    SHOULD_EQUAL(status4, JAS_LONG_MODE_INSTRUCTION);

    const jasErrorCode_t status5 = jasCodegen(ADC, rax, imm32, JAS_NO_OPERAND, JAS_NO_OPERAND, &longModeInstance);

    SHOULD_EQUAL(status5, JAS_NO_ERROR);
    SHOULD_EQUAL(longModeInstance.buffer[0], 0x48);
    SHOULD_EQUAL(longModeInstance.buffer[1], RAX_IMM32);
    SHOULD_EQUAL(longModeInstance.buffer[2], 0xFF);
    SHOULD_EQUAL(longModeInstance.buffer[3], 0xFF);
    SHOULD_EQUAL(longModeInstance.buffer[4], 0xFF);
    SHOULD_EQUAL(longModeInstance.buffer[5], 0xFF);
  });

  ({
    const jasReg8_t reg8 = JAS_REG_CL;
    const jasReg16_t reg16 = JAS_REG_BX;
    const jasReg32_t reg32 = JAS_REG_EBX;
    const jasReg64_t reg64 = JAS_REG_RBX;

    const uint8_t val8 = 0xFF;
    const uint16_t val16 = 0xFFFF;
    const uint32_t val32 = 0xFFFFFFFF;

    const jasTaggedOperand_t cl = jasConstructOperand(&reg8, JAS_REG_OPERAND_8);
    const jasTaggedOperand_t bx = jasConstructOperand(&reg16, JAS_REG_OPERAND_16);
    const jasTaggedOperand_t ebx = jasConstructOperand(&reg32, JAS_REG_OPERAND_32);
    const jasTaggedOperand_t rbx = jasConstructOperand(&reg64, JAS_REG_OPERAND_64);

    const jasTaggedOperand_t imm8 = jasConstructOperand(&val8, JAS_OPERAND_8);
    const jasTaggedOperand_t imm16 = jasConstructOperand(&val16, JAS_OPERAND_16);
    const jasTaggedOperand_t imm32 = jasConstructOperand(&val32, JAS_OPERAND_32);

    const jasErrorCode_t status = jasCodegen(ADC, cl, imm8, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[11], REG8_IMM8);
    SHOULD_EQUAL(realModeInstance.buffer[12], 0xD1);
    SHOULD_EQUAL(realModeInstance.buffer[13], 0xFF);

    const jasErrorCode_t status2 = jasCodegen(ADC, bx, imm16, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status2, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[14], 0x66);
    SHOULD_EQUAL(realModeInstance.buffer[15], REG16_IMM16);
    SHOULD_EQUAL(realModeInstance.buffer[16], 0xD3);
    SHOULD_EQUAL(realModeInstance.buffer[17], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[18], 0xFF);

    const jasErrorCode_t status3 = jasCodegen(ADC, ebx, imm32, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status3, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[19], REG32_IMM32);
    SHOULD_EQUAL(realModeInstance.buffer[20], 0xD3);
    SHOULD_EQUAL(realModeInstance.buffer[21], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[22], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[23], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[24], 0xFF);

    const jasErrorCode_t status4 = jasCodegen(ADC, rbx, imm32, JAS_NO_OPERAND, JAS_NO_OPERAND, &tempRealModeInstance);

    SHOULD_EQUAL(status4, JAS_LONG_MODE_INSTRUCTION);

    const jasErrorCode_t status5 = jasCodegen(ADC, rbx, imm32, JAS_NO_OPERAND, JAS_NO_OPERAND, &longModeInstance);

    SHOULD_EQUAL(status5, JAS_NO_ERROR);
    SHOULD_EQUAL(longModeInstance.buffer[6], 0x48);
    SHOULD_EQUAL(longModeInstance.buffer[7], REG64_IMM32);
    SHOULD_EQUAL(longModeInstance.buffer[8], 0xD3);
    SHOULD_EQUAL(longModeInstance.buffer[9], 0xFF);
    SHOULD_EQUAL(longModeInstance.buffer[10], 0xFF);
    SHOULD_EQUAL(longModeInstance.buffer[11], 0xFF);
    SHOULD_EQUAL(longModeInstance.buffer[12], 0xFF);
  });

  ({
    const jasReg8_t reg8 = JAS_REG_CL;
    const jasReg16_t reg16 = JAS_REG_BX;
    const jasReg32_t reg32 = JAS_REG_EBX;
    const jasReg64_t reg64 = JAS_REG_RBX;

    const uint8_t val8 = 0xFF;
    const uint16_t val16 = 0xFFFF;
    const uint32_t val32 = 0xFFFFFFFF;

    const jasTaggedOperand_t cl = jasConstructOperand(&reg8, JAS_REG_OPERAND_8);
    const jasTaggedOperand_t bx = jasConstructOperand(&reg16, JAS_REG_OPERAND_16);
    const jasTaggedOperand_t ebx = jasConstructOperand(&reg32, JAS_REG_OPERAND_32);
    const jasTaggedOperand_t rbx = jasConstructOperand(&reg64, JAS_REG_OPERAND_64);

    const jasTaggedOperand_t imm8 = jasConstructOperand(&val8, JAS_OPERAND_8);
    const jasTaggedOperand_t imm16 = jasConstructOperand(&val16, JAS_OPERAND_16);
    const jasTaggedOperand_t imm32 = jasConstructOperand(&val32, JAS_OPERAND_32);

    const jasErrorCode_t status2 = jasCodegen(ADC, bx, imm8, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status2, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[25], 0x66);
    SHOULD_EQUAL(realModeInstance.buffer[26], REG16_IMM8);
    SHOULD_EQUAL(realModeInstance.buffer[27], 0xD3);
    SHOULD_EQUAL(realModeInstance.buffer[28], 0xFF);

    const jasErrorCode_t status3 = jasCodegen(ADC, ebx, imm8, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status3, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[29], REG32_IMM8);
    SHOULD_EQUAL(realModeInstance.buffer[30], 0xD3);
    SHOULD_EQUAL(realModeInstance.buffer[31], 0xFF);

    const jasErrorCode_t status4 = jasCodegen(ADC, rbx, imm8, JAS_NO_OPERAND, JAS_NO_OPERAND, &tempRealModeInstance);

    SHOULD_EQUAL(status4, JAS_LONG_MODE_INSTRUCTION);

    const jasErrorCode_t status5 = jasCodegen(ADC, rbx, imm8, JAS_NO_OPERAND, JAS_NO_OPERAND, &longModeInstance);

    SHOULD_EQUAL(status5, JAS_NO_ERROR);
    SHOULD_EQUAL(longModeInstance.buffer[13], 0x48);
    SHOULD_EQUAL(longModeInstance.buffer[14], REG64_IMM8);
    SHOULD_EQUAL(longModeInstance.buffer[15], 0xD3);
    SHOULD_EQUAL(longModeInstance.buffer[16], 0xFF);
  });

  ({
    const jasReg8_t reg8 = JAS_REG_CL;
    const jasReg16_t reg16 = JAS_REG_BX;
    const jasReg32_t reg32 = JAS_REG_EBX;
    const jasReg64_t reg64 = JAS_REG_RBX;

    const uint8_t val8 = 0xFF;
    const uint16_t val16 = 0xFFFF;
    const uint32_t val32 = 0xFFFFFFFF;

    const jasTaggedOperand_t cl = jasConstructOperand(&reg8, JAS_REG_OPERAND_8);
    const jasTaggedOperand_t bx = jasConstructOperand(&reg16, JAS_REG_OPERAND_16);
    const jasTaggedOperand_t ebx = jasConstructOperand(&reg32, JAS_REG_OPERAND_32);
    const jasTaggedOperand_t rbx = jasConstructOperand(&reg64, JAS_REG_OPERAND_64);

    const jasErrorCode_t status = jasCodegen(ADC, cl, cl, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status, JAS_NO_ERROR)
    SHOULD_EQUAL(realModeInstance.buffer[32], REG8_REG8)
    SHOULD_EQUAL(realModeInstance.buffer[33], 0xC9)

    const jasErrorCode_t status2 = jasCodegen(ADC, bx, bx, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status, JAS_NO_ERROR)
    SHOULD_EQUAL(realModeInstance.buffer[34], 0x66)
    SHOULD_EQUAL(realModeInstance.buffer[35], REG16_REG16)
    SHOULD_EQUAL(realModeInstance.buffer[36], 0xDB)

    const jasErrorCode_t status3 = jasCodegen(ADC, ebx, ebx, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status, JAS_NO_ERROR)
    SHOULD_EQUAL(realModeInstance.buffer[37], REG32_REG32)
    SHOULD_EQUAL(realModeInstance.buffer[38], 0xDB)

    const jasErrorCode_t status4 = jasCodegen(ADC, rbx, rbx, JAS_NO_OPERAND, JAS_NO_OPERAND, &tempRealModeInstance);

    SHOULD_EQUAL(status4, JAS_LONG_MODE_INSTRUCTION);

    const jasErrorCode_t status5 = jasCodegen(ADC, rbx, rbx, JAS_NO_OPERAND, JAS_NO_OPERAND, &longModeInstance);

    SHOULD_EQUAL(status5, JAS_NO_ERROR);
    SHOULD_EQUAL(longModeInstance.buffer[17], 0x48);
    SHOULD_EQUAL(longModeInstance.buffer[18], REG64_REG64);
    SHOULD_EQUAL(longModeInstance.buffer[19], 0xDB);

    /**
     * @note Please note that some cases with the `0x13` and `0x12` opcodes
     * are not tested due to the incompatible r/m and reg combinations of the instruction
     * drivers.
     *
     * - Alvin
     */
  });

  free(realModeInstance.buffer);
  free(longModeInstance.buffer);
  free(tempRealModeInstance.buffer);
  free(tempRealModeInstance.buffer);

  PASS
}
