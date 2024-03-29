#include "codegen.h"
#include "init.h"
#include "labrador/labrador.h"
#include "null.h"
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

    const jasErrorCode_t status = jasCodegen(ADD, al, imm8, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[0], 0x04);
    SHOULD_EQUAL(realModeInstance.buffer[1], 0xFF);

    const jasErrorCode_t status2 = jasCodegen(ADD, ax, imm16, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status2, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[2], 0x66);
    SHOULD_EQUAL(realModeInstance.buffer[3], 0x05);
    SHOULD_EQUAL(realModeInstance.buffer[4], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[5], 0xFF);

    const jasErrorCode_t status3 = jasCodegen(ADD, eax, imm32, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status3, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[6], 0x05);
    SHOULD_EQUAL(realModeInstance.buffer[7], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[8], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[9], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[10], 0xFF);

    const jasErrorCode_t status4 = jasCodegen(ADD, rax, imm32, JAS_NO_OPERAND, JAS_NO_OPERAND, &tempRealModeInstance);

    SHOULD_EQUAL(status4, JAS_LONG_MODE_INSTRUCTION);

    const jasErrorCode_t status5 = jasCodegen(ADD, rax, imm32, JAS_NO_OPERAND, JAS_NO_OPERAND, &longModeInstance);

    SHOULD_EQUAL(status5, JAS_NO_ERROR);
    SHOULD_EQUAL(longModeInstance.buffer[0], 0x48);
    SHOULD_EQUAL(longModeInstance.buffer[1], 0x05);
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

    const jasErrorCode_t status = jasCodegen(ADD, cl, imm8, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[11], 0x80);
    SHOULD_EQUAL(realModeInstance.buffer[12], 0xC1);
    SHOULD_EQUAL(realModeInstance.buffer[13], 0xFF);

    const jasErrorCode_t status2 = jasCodegen(ADD, bx, imm16, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status2, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[14], 0x66);
    SHOULD_EQUAL(realModeInstance.buffer[15], 0x81);
    SHOULD_EQUAL(realModeInstance.buffer[16], 0xC3);
    SHOULD_EQUAL(realModeInstance.buffer[17], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[18], 0xFF);

    const jasErrorCode_t status3 = jasCodegen(ADD, ebx, imm32, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status3, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[19], 0x81);
    SHOULD_EQUAL(realModeInstance.buffer[20], 0xC3);
    SHOULD_EQUAL(realModeInstance.buffer[21], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[22], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[23], 0xFF);
    SHOULD_EQUAL(realModeInstance.buffer[24], 0xFF);

    const jasErrorCode_t status4 = jasCodegen(ADD, rbx, imm32, JAS_NO_OPERAND, JAS_NO_OPERAND, &tempRealModeInstance);

    SHOULD_EQUAL(status4, JAS_LONG_MODE_INSTRUCTION);

    const jasErrorCode_t status5 = jasCodegen(ADD, rbx, imm32, JAS_NO_OPERAND, JAS_NO_OPERAND, &longModeInstance);

    SHOULD_EQUAL(status5, JAS_NO_ERROR);
    SHOULD_EQUAL(longModeInstance.buffer[6], 0x48);
    SHOULD_EQUAL(longModeInstance.buffer[7], 0x81);
    SHOULD_EQUAL(longModeInstance.buffer[8], 0xC3);
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

    const jasErrorCode_t status2 = jasCodegen(ADD, bx, imm8, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status2, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[25], 0x66);
    SHOULD_EQUAL(realModeInstance.buffer[26], 0x83);
    SHOULD_EQUAL(realModeInstance.buffer[27], 0xC3);
    SHOULD_EQUAL(realModeInstance.buffer[28], 0xFF);

    const jasErrorCode_t status3 = jasCodegen(ADD, ebx, imm8, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status3, JAS_NO_ERROR);
    SHOULD_EQUAL(realModeInstance.buffer[29], 0x83);
    SHOULD_EQUAL(realModeInstance.buffer[30], 0xC3);
    SHOULD_EQUAL(realModeInstance.buffer[31], 0xFF);

    const jasErrorCode_t status4 = jasCodegen(ADD, rbx, imm8, JAS_NO_OPERAND, JAS_NO_OPERAND, &tempRealModeInstance);

    SHOULD_EQUAL(status4, JAS_LONG_MODE_INSTRUCTION);

    const jasErrorCode_t status5 = jasCodegen(ADD, rbx, imm8, JAS_NO_OPERAND, JAS_NO_OPERAND, &longModeInstance);

    SHOULD_EQUAL(status5, JAS_NO_ERROR);
    SHOULD_EQUAL(longModeInstance.buffer[13], 0x48);
    SHOULD_EQUAL(longModeInstance.buffer[14], 0x83);
    SHOULD_EQUAL(longModeInstance.buffer[15], 0xC3);
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

    const jasErrorCode_t status = jasCodegen(ADD, cl, cl, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status, JAS_NO_ERROR)
    SHOULD_EQUAL(realModeInstance.buffer[32], 0x02)
    SHOULD_EQUAL(realModeInstance.buffer[33], 0xC9)

    const jasErrorCode_t status2 = jasCodegen(ADD, bx, bx, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status, JAS_NO_ERROR)
    SHOULD_EQUAL(realModeInstance.buffer[34], 0x66)
    SHOULD_EQUAL(realModeInstance.buffer[35], 0x03)
    SHOULD_EQUAL(realModeInstance.buffer[36], 0xDB)

    const jasErrorCode_t status3 = jasCodegen(ADD, ebx, ebx, JAS_NO_OPERAND, JAS_NO_OPERAND, &realModeInstance);

    SHOULD_EQUAL(status, JAS_NO_ERROR)
    SHOULD_EQUAL(realModeInstance.buffer[37], 0x03)
    SHOULD_EQUAL(realModeInstance.buffer[38], 0xDB)

    const jasErrorCode_t status4 = jasCodegen(ADD, rbx, rbx, JAS_NO_OPERAND, JAS_NO_OPERAND, &tempRealModeInstance);

    SHOULD_EQUAL(status4, JAS_LONG_MODE_INSTRUCTION);

    const jasErrorCode_t status5 = jasCodegen(ADD, rbx, rbx, JAS_NO_OPERAND, JAS_NO_OPERAND, &longModeInstance);

    SHOULD_EQUAL(status5, JAS_NO_ERROR);
    SHOULD_EQUAL(longModeInstance.buffer[17], 0x48);
    SHOULD_EQUAL(longModeInstance.buffer[18], 0x03);
    SHOULD_EQUAL(longModeInstance.buffer[19], 0xDB);
  });

  free(realModeInstance.buffer);
  free(longModeInstance.buffer);
  free(tempRealModeInstance.buffer);

  PASS
}
