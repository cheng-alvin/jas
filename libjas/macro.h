#ifndef MACRO_H
#define MACRO_H

#define WRITE(x)                                                       \
  instance->buffer = write8(instance->buffer, x, instance->bufferLen); \
  instance->bufferLen++;

#endif