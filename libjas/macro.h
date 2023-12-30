#ifndef MACRO_H
#define MACRO_H

// Internal macros for less spaghetti code 🍝.
// Delicious!!

#define WRITE(x)                                                       \
  instance->buffer = write8(instance->buffer, x, instance->bufferLen); \
  instance->bufferLen++;

#define WRITE_LE_16(x)                                                  \
  instance->buffer = write16(instance->buffer, x, instance->bufferLen); \
  instance->bufferLen += 2;

#define WRITE_LE_32(x)            \
  WRITE_LE_16((x >> 16) & 0xffff) \
  WRITE_LE_16(x & 0xffff)

#define WRITE_LE_64(x)            \
  WRITE_LE_16((x >> 48) & 0xffff) \
  WRITE_LE_16((x >> 32) & 0xffff) \
  WRITE_LE_16((x >> 16) & 0xffff) \
  WRITE_LE_16(x & 0xffff)

#define WRITE_IF(x, y) \
  if (x) {             \
    WRITE(y)           \
  }

#define WRITE_LE_16_IF(x, y) \
  if (x) {                   \
    WRITE_LE_16(y)           \
  }

#define WRITE_LE_32_IF(x, y) \
  if (x) {                   \
    WRITE_LE_32(y)           \
  }

#define WRITE_LE_64_IF(x, y) \
  if (x) {                   \
    WRITE_LE_64(y)           \
  }

#define WRITE_IF_BREAKS(x, y) \
  if (x) {                    \
    WRITE(y)                  \
    break;                    \
  }

#define WRITE_LE_16_IF_BREAKS(x, y) \
  if (x) {                          \
    WRITE_LE_16(y)                  \
    break;                          \
  }

#define WRITE_LE_32_IF_BREAKS(x, y) \
  if (x) {                          \
    WRITE_LE_32(y)                  \
    break;                          \
  }

#define WRITE_LE_64_IF_BREAKS(x, y) \
  if (x) {                          \
    WRITE_LE_64(y)                  \
    break;                          \
  }

#define NO_ERROR return JAS_NO_ERROR;
#define NO_LONG_MODE                 \
  if (instance->mode == JAS_MODE_64) \
    return JAS_NON_LONG_MODE_INSTRUCTION;

#define ENCODE_IMMEDIATE(x)                           \
  switch (x.type) {                                   \
  case JAS_OPERAND_8:                                 \
    WRITE(x.operand.operand8);                        \
    break;                                            \
  case JAS_OPERAND_16:                                \
    WRITE_LE_16(x.operand.operand16);                 \
    break;                                            \
  case JAS_OPERAND_32:                                \
    WRITE_LE_16((x.operand.operand32 >> 16) & 0xffff) \
    WRITE_LE_16(x.operand.operand32 & 0xffff)         \
    break;                                            \
  case JAS_OPERAND_64:                                \
    WRITE_LE_16((x.operand.operand64 >> 48) & 0xffff) \
    WRITE_LE_16((x.operand.operand64 >> 32) & 0xffff) \
    WRITE_LE_16((x.operand.operand64 >> 16) & 0xffff) \
    WRITE_LE_16(x.operand.operand64 & 0xffff)         \
    break;                                            \
  }

#define IS_REG(x) (x.type == JAS_REG_OPERAND_8 || x.type == JAS_REG_OPERAND_16 || x.type == JAS_REG_OPERAND_32 || x.type == JAS_REG_OPERAND_64)

#define IS_INDIRECT(x) (x.type == JAS_INDIRECT_8 || x.type == JAS_INDIRECT_16 || x.type == JAS_INDIRECT_32 || x.type == JAS_INDIRECT_64)

#define IS_IMMEDIATE(x) (x.type == JAS_OPERAND_8 || x.type == JAS_OPERAND_16 || x.type == JAS_OPERAND_32 || x.type == JAS_OPERAND_64)

#endif