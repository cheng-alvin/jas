#include "instruction.h"

#include "pre.c"

#define ZERO_EXT 0b10000000
#define DEFINE_TAB(name) instr_encode_table_t name[]

DEFINE_TAB(mov) = {
    {ENC_MR, NULL, {0x89}, {0x88}, 1, &same_operand_sizes, true},
    {ENC_RM, NULL, {0x8B}, {0x8A}, 1, &same_operand_sizes, true},
    {ENC_OI, NULL, {0xB8}, {0xB0}, 1, &same_operand_sizes},
    {ENC_MI, ZERO_EXT, {0xC7}, {0xC6}, 1, &pre_imm, true},

    INSTR_TAB_NULL,
};

// clang-format off

#define GENERIC(rm, rm_byte, mr, mr_byte,i, i_byte, mi_ext, mi, mi_byte)  \
     {ENC_RM, NULL, {rm},  {rm_byte}, 1, &same_operand_sizes, true},        \
      {ENC_MR, NULL, {mr},  {mr_byte}, 1, &same_operand_sizes, true},        \
      {ENC_I, NULL, {i},  {i_byte}, 1, &pre_imm, true},               \
      {ENC_MI, mi_ext, {mi},  {mi_byte}, 1, &pre_imm, true},          \
      INSTR_TAB_NULL,


DEFINE_TAB(lea) = {{ENC_RM, NULL, {0x8D},  {0x8D}, 1, &pre_lea, true}, INSTR_TAB_NULL};

DEFINE_TAB(add) = { GENERIC(0x03, 0x02, 0x01, 0x00, 0x03, 0x02, ZERO_EXT, 0x81, 0x80) };
DEFINE_TAB(sub) = { GENERIC(0x2B, 0x2A, 0x28, 0x29, 0x2C, 0x2D, 5, 0x81, 0x80) };
DEFINE_TAB(mul) = {{ENC_M, 4, {0xF7},  {0xF6}, 1, &same_operand_sizes, true}, INSTR_TAB_NULL};
DEFINE_TAB(_div) = {{ENC_M, 6, {0xF7},  {0xF6}, 1, &same_operand_sizes, true}, INSTR_TAB_NULL};

// Note all or, and and xor instructions have a imm8 which is not supported

DEFINE_TAB(and) = { GENERIC(0x23, 0x22, 0x21, 0x20, 0x25, 0x24, 4, 0x81, 0x80) };
DEFINE_TAB(or)  = { GENERIC(0x0B, 0x0A, 0x09, 0x08, 0x0D, 0x0C, 1, 0x81, 0x80) };
DEFINE_TAB(xor) = { GENERIC(0x33, 0x32, 0x31, 0x30, 0x35, 0x34, 6, 0x81, 0x80) };

// clang-format on
// ---

DEFINE_TAB(_not) = {{ENC_M, 2, {0xF7}, {0xF6}, 1, &same_operand_sizes, true}, INSTR_TAB_NULL};

DEFINE_TAB(inc) = {{ENC_M, 0, {0xFF}, {0xFE}, 1, &same_operand_sizes, true}, INSTR_TAB_NULL};
DEFINE_TAB(dec) = {{ENC_M, 1, {0xFF}, {0xFE}, 1, &same_operand_sizes, true}, INSTR_TAB_NULL};

DEFINE_TAB(jmp) = {
    {ENC_D, NULL, {0xE9}, {0xEB}, 1, NULL, true},
    {ENC_M, 4, {0xFF}, {NULL}, 1, &pre_imm, false},
    INSTR_TAB_NULL,
};

DEFINE_TAB(je) = {{ENC_D, NULL, {0x0f, 0x84}, {0x090, 0x74}, 2, NULL, true}, INSTR_TAB_NULL};
DEFINE_TAB(jne) = {{ENC_D, NULL, {0x0f, 0x85}, {0x00, 0x00}, 2, &pre_jcc_no_byte, true}, INSTR_TAB_NULL};
DEFINE_TAB(jz) = {{ENC_D, NULL, {0x0f, 0x84}, {0x00, 0x00}, 2, &pre_jcc_no_byte, true}, INSTR_TAB_NULL};
DEFINE_TAB(jnz) = {{ENC_D, NULL, {0x0f, 0x85}, {0x90, 0x75}, 2, NULL, true}, INSTR_TAB_NULL};

DEFINE_TAB(call) = {
    {ENC_D, NULL, {0xE8}, {0xEB}, 1, NULL, true},
    {ENC_M, 2, {0xFF}, {NULL}, 1, &pre_imm, false},
    INSTR_TAB_NULL,
};

// TODO / note far jumps, calls and returns are not supported (yet)
DEFINE_TAB(ret) = {
    {ENC_ZO, NULL, {0xC3}, {0xC3}, 1, &no_operands, true},
    {ENC_I, NULL, {0xC2}, {0xC2}, 1, &pre_ret, true},
    INSTR_TAB_NULL,
};

DEFINE_TAB(cmp) = {GENERIC(0x3B, 0x3A, 0x39, 0x38, 0x3D, 0x3C, 7, 0x81, 0x80)};

DEFINE_TAB(push) = {
    {ENC_M, 6, {0xFF}, NULL, 1, NULL, false},
    {ENC_O, NULL, {0x50}, NULL, 1, NULL, false},
    {ENC_I, NULL, {0x68}, {0x6A}, 1, &pre_imm, true},
    INSTR_TAB_NULL,
};

DEFINE_TAB(pop) = {
    {ENC_M, 0, {0x8F}, NULL, 1, NULL, false},
    {ENC_O, NULL, {0x58}, NULL, 1, NULL, false},
    INSTR_TAB_NULL,
};

DEFINE_TAB(in) = {
    {ENC_OI, NULL, {0xE5}, {0xE4}, 1, &pre_in_out, true},
    {ENC_IGN, NULL, {0xED}, {0xEC}, 1, &pre_in_out, true},
    INSTR_TAB_NULL,
};

// Note the jumble up of the operands in the table, OUT imm and AL is CORRECT instead.
DEFINE_TAB(out) = {
    {ENC_OI, NULL, {0xE7}, {0xE6}, 1, &pre_in_out, true},
    {ENC_IGN, NULL, {0xEF}, {0xEE}, 1, &pre_in_out, true},
    INSTR_TAB_NULL,
};

#define ZO_GENERIC(opcode) \
  {ENC_ZO, NULL, {opcode}, {NULL}, 1, &no_operands, false}, INSTR_TAB_NULL

// clang-format off

DEFINE_TAB(clc) = { ZO_GENERIC(0xF8) }; DEFINE_TAB(stc) = { ZO_GENERIC(0xF9) };
DEFINE_TAB(cli) = { ZO_GENERIC(0xFA) }; DEFINE_TAB(sti) = { ZO_GENERIC(0xFB) };
DEFINE_TAB(nop) = { ZO_GENERIC(0x90) }; DEFINE_TAB(hlt) = { ZO_GENERIC(0xF4) };

// clang-format on

#undef ZO_GENERIC

DEFINE_TAB(_int) = {{ENC_I, NULL, {0xCD}, {NULL}, 1, &pre_int, false}, INSTR_TAB_NULL};

DEFINE_TAB(syscall) = {{ENC_ZO, NULL, {0x0F, 0x05}, {NULL}, 2, &same_operand_sizes, false}, INSTR_TAB_NULL};
DEFINE_TAB(movzx) = {{ENC_RM, NULL, {0x0F, 0xB7}, {0x0F, 0xB6}, 2, &pre_small_operands, true}, INSTR_TAB_NULL};
DEFINE_TAB(movsx) = {{ENC_RM, NULL, {0x0F, 0xBF}, {0x0F, 0xBE}, 2, &pre_small_operands, true}, INSTR_TAB_NULL};

DEFINE_TAB(xchg) = {
    {ENC_O, NULL, {0x90}, {NULL}, 1, &same_operand_sizes, false},
    {ENC_MR, NULL, {0x87}, {0x86}, 1, &same_operand_sizes, true},
    {ENC_RM, NULL, {0x87}, {0x86}, 1, &same_operand_sizes, true},
    INSTR_TAB_NULL,
};

DEFINE_TAB(bswap) = {{ENC_O, NULL, {0x0F, 0xC8}, {NULL}, 2, &same_operand_sizes, false}, INSTR_TAB_NULL};

#define CMOV_CC(opcode) \
  {ENC_RM, NULL, {0x0F, opcode}, {NULL}, 2, &pre_cmov, false}, INSTR_TAB_NULL

// For reference, see above
// TODO Confirm correctness

// clang-format off

DEFINE_TAB(cmova)    = { CMOV_CC(0x47) };  DEFINE_TAB(cmovae)   = { CMOV_CC(0x43) };
DEFINE_TAB(cmovb)    = { CMOV_CC(0x42) };  DEFINE_TAB(cmovbe)   = { CMOV_CC(0x46) };
DEFINE_TAB(cmove)    = { CMOV_CC(0x44) };  DEFINE_TAB(cmovg)    = { CMOV_CC(0x4F) };
DEFINE_TAB(cmovge)   = { CMOV_CC(0x4D) };  DEFINE_TAB(cmovl)    = { CMOV_CC(0x4C) };
DEFINE_TAB(cmovle)   = { CMOV_CC(0x4E) };  DEFINE_TAB(cmovna)   = { CMOV_CC(0x46) };
DEFINE_TAB(cmovnae)  = { CMOV_CC(0x42) };  DEFINE_TAB(cmovnb)   = { CMOV_CC(0x43) };
DEFINE_TAB(cmovnbe)  = { CMOV_CC(0x47) };  DEFINE_TAB(cmovne)   = { CMOV_CC(0x45) };
DEFINE_TAB(cmovng)   = { CMOV_CC(0x4E) };  DEFINE_TAB(cmovnge)  = { CMOV_CC(0x4C) };
DEFINE_TAB(cmovnl)   = { CMOV_CC(0x4D) };  DEFINE_TAB(cmovnle)  = { CMOV_CC(0x4F) };
DEFINE_TAB(cmovno)   = { CMOV_CC(0x41) };  DEFINE_TAB(cmovnp)   = { CMOV_CC(0x4B) };
DEFINE_TAB(cmovns)   = { CMOV_CC(0x49) };  DEFINE_TAB(cmovnz)   = { CMOV_CC(0x45) };
DEFINE_TAB(cmovo)    = { CMOV_CC(0x40) };  DEFINE_TAB(cmovp)    = { CMOV_CC(0x4A) };
DEFINE_TAB(cmovpe)   = { CMOV_CC(0x4A) };  DEFINE_TAB(cmovpo)   = { CMOV_CC(0x4B) };
DEFINE_TAB(cmovs)    = { CMOV_CC(0x48) };  DEFINE_TAB(cmovz)    = { CMOV_CC(0x44) };

// clang-format on

#undef ZERO_EXT
#undef CMOV_CC