#include "instruction.h"
#include "pre.c"

instr_encode_table_t mov[] = {
    {ENC_MR, NULL, {0x89}, {0x88}, 1, &same_operand_sizes, 1},
    {ENC_RM, NULL, {0x8B}, {0x8A}, 1, &same_operand_sizes, 1},
    {ENC_OI, NULL, {0xB8}, {0xB0}, 1, &same_operand_sizes, 1},
    {ENC_MI, 0b10000000, {0xC7}, {0xC6}, 1, &pre_imm, 1},
    INSTR_TAB_NULL,
};
instr_encode_table_t lea[] = {
    {ENC_RM, NULL, {0x8D}, {NULL}, 1, NULL, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t add[] = {
    {ENC_RM, NULL, {0x03}, {0x02}, 1, &same_operand_sizes, 1},
    {ENC_MR, NULL, {0x01}, {0x00}, 1, &same_operand_sizes, 1},
    {ENC_I, NULL, {0x05}, {0x04}, 1, &pre_imm, 1},
    {ENC_MI, 0b10000000, {0x81}, {0x80}, 1, &pre_imm, 1},
    INSTR_TAB_NULL,
};
instr_encode_table_t sub[] = {
    {ENC_RM, NULL, {0x2b}, {0x2a}, 1, &same_operand_sizes, 1},
    {ENC_MR, NULL, {0x29}, {0x28}, 1, &same_operand_sizes, 1},
    {ENC_I, NULL, {0x2c}, {0x2d}, 1, &pre_imm, 1},
    {ENC_MI, 8, {0x80}, {0x81}, 1, &pre_imm, 1},
    INSTR_TAB_NULL,
};
instr_encode_table_t mul[] = {
    {ENC_M, 4, {0xF7}, {0xF6}, 1, NULL, 1},
    INSTR_TAB_NULL,
};
instr_encode_table_t _div[] = {
    {ENC_M, 6, {0xF7}, {0xF6}, 1, NULL, 1},
    INSTR_TAB_NULL,
};
instr_encode_table_t and[] = {
    {ENC_RM, NULL, {0x23}, {0x22}, 1, &same_operand_sizes, 1},
    {ENC_MR, NULL, {0x21}, {0x20}, 1, &same_operand_sizes, 1},
    {ENC_I, NULL, {0x25}, {0x24}, 1, &pre_imm, 1},
    {ENC_MI, 1, {0x81}, {0x80}, 1, &pre_imm, 1},
    INSTR_TAB_NULL,
};
instr_encode_table_t or [] = {
    {ENC_RM, NULL, {0x0B}, {0x0A}, 1, &same_operand_sizes, 1},
    {ENC_MR, NULL, {0x09}, {0x08}, 1, &same_operand_sizes, 1},
    {ENC_I, NULL, {0x0d}, {0x0c}, 1, &same_operand_sizes, 1},
    {ENC_MI, 1, {0x81}, {0x80}, 1, &pre_imm, 1},
    INSTR_TAB_NULL,
};
instr_encode_table_t xor [] = {
    {ENC_RM, NULL, {0x33}, {0x32}, 1, &same_operand_sizes, 1},
    {ENC_MR, NULL, {0x31}, {0x30}, 1, &same_operand_sizes, 1},
    {ENC_I, NULL, {0x35}, {0x34}, 1, &pre_imm, 1},
    {ENC_MI, 6, {0x81}, {0x80}, 1, &pre_imm, 1},
    INSTR_TAB_NULL,
};
instr_encode_table_t _not[] = {
    {ENC_M, 2, {0xF7}, {0xF6}, 1, &same_operand_sizes, 1},
    INSTR_TAB_NULL,
};
instr_encode_table_t inc[] = {
    {ENC_M, 0, {0xFF}, {0xFE}, 1, &same_operand_sizes, 1},
    INSTR_TAB_NULL,
};
instr_encode_table_t dec[] = {
    {ENC_M, 1, {0xFF}, {0xFE}, 1, &same_operand_sizes, 1},
    INSTR_TAB_NULL,
};
instr_encode_table_t jmp[] = {
    {ENC_D, NULL, {0xE9}, {0xEB}, 1, NULL, 1},
    {ENC_M, 4, {0xFF}, {NULL}, 1, &pre_imm, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t je[] = {
    {ENC_D, NULL, {0x0f, 0x84}, {0x74}, 2, NULL, 1},
    INSTR_TAB_NULL,
};
instr_encode_table_t jnz[] = {
    {ENC_D, NULL, {0x0f, 0x85}, {0x75}, 2, NULL, 1},
    INSTR_TAB_NULL,
};
instr_encode_table_t jne[] = {
    {ENC_D, NULL, {0x0f, 0x85}, {NULL}, 2, &pre_jcc_no_byte, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t jz[] = {
    {ENC_D, NULL, {0x0f, 0x84}, {NULL}, 2, &pre_jcc_no_byte, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t call[] = {
    {ENC_D, NULL, {0xE8}, {0xEB}, 1, NULL, 1},
    {ENC_M, 2, {0xFF}, {NULL}, 1, &pre_imm, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t ret[] = {
    {ENC_ZO, NULL, {0xC3}, {NULL}, 1, &no_operands, 0},
    {ENC_I, NULL, {0xC2}, {NULL}, 1, &pre_ret, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmp[] = {
    {ENC_RM, NULL, {0x3B}, {0x3A}, 1, &same_operand_sizes, 1},
    {ENC_MR, NULL, {0x39}, {0x38}, 1, &same_operand_sizes, 1},
    {ENC_I, NULL, {0x3D}, {0x3C}, 1, &pre_imm, 1},
    {ENC_MI, 8, {0x81}, {0x80}, 1, &pre_imm, 1},
    INSTR_TAB_NULL,
};
instr_encode_table_t push[] = {
    {ENC_M, 6, {0xFF}, {NULL}, 1, NULL, 0},
    {ENC_O, NULL, {0x50}, {NULL}, 1, NULL, 0},
    {ENC_I, NULL, {0x68}, {0x6A}, 1, &pre_imm, 1},
    INSTR_TAB_NULL,
};
instr_encode_table_t pop[] = {
    {ENC_M, 0, {0x8F}, {NULL}, 1, NULL, 0},
    {ENC_O, NULL, {0x58}, {NULL}, 1, NULL, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t in[] = {
    {ENC_OI, NULL, {0xE5}, {0xE4}, 1, &pre_in_out, 1},
    {ENC_IGN, NULL, {0xED}, {0xEC}, 1, &pre_in_out, 1},
    INSTR_TAB_NULL,
};
instr_encode_table_t out[] = {
    {ENC_IGN, NULL, {0xEF}, {0xEE}, 1, &pre_in_out, 1},
    {ENC_OI, NULL, {0xE7}, {0xE6}, 1, &pre_in_out, 1},
    INSTR_TAB_NULL,
};
instr_encode_table_t clc[] = {
    {ENC_ZO, NULL, {0xF8}, {NULL}, 1, &no_operands, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t stc[] = {
    {ENC_ZO, NULL, {0xF9}, {NULL}, 1, &no_operands, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cli[] = {
    {ENC_ZO, NULL, {0xFA}, {NULL}, 1, &no_operands, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t sti[] = {
    {ENC_ZO, NULL, {0xFB}, {NULL}, 1, &no_operands, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t nop[] = {
    {ENC_ZO, NULL, {0x90}, {NULL}, 1, &no_operands, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t hlt[] = {
    {ENC_ZO, NULL, {0xF4}, {NULL}, 1, &no_operands, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t _int[] = {
    {ENC_I, NULL, {0xCD}, {NULL}, 1, &pre_int, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t xchg[] = {
    {ENC_O, NULL, {0x90}, {NULL}, 1, &same_operand_sizes, 0},
    {ENC_MR, NULL, {0x87}, {0x86}, 1, &same_operand_sizes, 1},
    {ENC_RM, NULL, {0x87}, {0x86}, 1, &same_operand_sizes, 1},
    INSTR_TAB_NULL,
};
instr_encode_table_t syscall[] = {
    {ENC_ZO, NULL, {0x0f}, {NULL}, 1, &no_operands, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t movzx[] = {
    {ENC_RM, NULL, {0x0F, 0xB7}, {0x0F, 0xB6}, 2, &pre_small_operands, 2},
    INSTR_TAB_NULL,
};
instr_encode_table_t movsx[] = {
    {ENC_RM, NULL, {0x0F, 0xBF}, {0x0F, 0xBE}, 2, &pre_small_operands, 2},
    INSTR_TAB_NULL,
};
instr_encode_table_t bswap[] = {
    {ENC_O, NULL, {0x0f, 0xC8}, {NULL}, 2, &same_operand_sizes, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmova[] = {
    {ENC_RM, NULL, {0x0F, 0x47}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovae[] = {
    {ENC_RM, NULL, {0x0F, 0x43}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovb[] = {
    {ENC_RM, NULL, {0x0F, 0x42}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovbe[] = {
    {ENC_RM, NULL, {0x0F, 0x46}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmove[] = {
    {ENC_RM, NULL, {0x0F, 0x44}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovg[] = {
    {ENC_RM, NULL, {0x0F, 0x4F}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovge[] = {
    {ENC_RM, NULL, {0x0F, 0x4D}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovl[] = {
    {ENC_RM, NULL, {0x0F, 0x4C}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovle[] = {
    {ENC_RM, NULL, {0x0F, 0x4E}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovna[] = {
    {ENC_RM, NULL, {0x0F, 0x46}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovnae[] = {
    {ENC_RM, NULL, {0x0F, 0x42}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovnb[] = {
    {ENC_RM, NULL, {0x0F, 0x43}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovnbe[] = {
    {ENC_RM, NULL, {0x0F, 0x47}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovne[] = {
    {ENC_RM, NULL, {0x0F, 0x45}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovng[] = {
    {ENC_RM, NULL, {0x0F, 0x4E}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovnge[] = {
    {ENC_RM, NULL, {0x0F, 0x4C}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovnl[] = {
    {ENC_RM, NULL, {0x0F, 0x4D}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovnle[] = {
    {ENC_RM, NULL, {0x0F, 0x4F}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovno[] = {
    {ENC_RM, NULL, {0x0F, 0x41}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovnp[] = {
    {ENC_RM, NULL, {0x0F, 0x4B}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovns[] = {
    {ENC_RM, NULL, {0x0F, 0x49}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovnz[] = {
    {ENC_RM, NULL, {0x0F, 0x45}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovo[] = {
    {ENC_RM, NULL, {0x0F, 0x40}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovp[] = {
    {ENC_RM, NULL, {0x0F, 0x4A}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovpe[] = {
    {ENC_RM, NULL, {0x0F, 0x4A}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovpo[] = {
    {ENC_RM, NULL, {0x0F, 0x4B}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovs[] = {
    {ENC_RM, NULL, {0x0F, 0x48}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
instr_encode_table_t cmovz[] = {
    {ENC_RM, NULL, {0x0F, 0x44}, {NULL}, 2, &pre_cmov, 0},
    INSTR_TAB_NULL,
};
