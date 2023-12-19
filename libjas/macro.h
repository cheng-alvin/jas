#ifndef MACRO_H
#define MACRO_H

#define WRITE(x)                                                       \
  instance->buffer = write8(instance->buffer, x, instance->bufferLen); \
  instance->bufferLen++;

#define WRITE_LE_16(x)                                                  \
  instance->buffer = write16(instance->buffer, x, instance->bufferLen); \
  instance->bufferLen++;

#define WRITE_IF(x, y) \
  if (x) {             \
    WRITE(y)           \
  }

#define WRITE_LE_16_IF(x, y) \
  if (x) {                   \
    WRITE_LE_16(y)           \
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

#define NO_ERROR return JAS_NO_ERROR;
#define NO_LONG_MODE                 \
  if (instance->mode == JAS_MODE_64) \
    return JAS_NON_LONG_MODE_INSTRUCTION;

#define ENCODE_RM(x) \
  jasGenerateModrm()
#endif
