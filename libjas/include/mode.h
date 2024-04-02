#ifndef MODE_H
#define MODE_H

/**
 * Enums representing the operating modes of
 * the x86 processor as defined by the Intel
 * manual.
 */
enum modes {
  MODE_REAL,
  MODE_PROTECTED,
  MODE_LONG,
};

#endif