/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Enum of 64 bit registers, used for the code generation and transpilation
 * processes.
 *
 * @note Only used for 64 bit registers, 32 bit registers are not
 * compatible with 64 bit registers and may require additional setup.
 */

enum registers64_t {
  RAX = 0x00,
  RBX = 0x03,
  RCX = 0x01,
  RDX = 0x02,
  RSI = 0x06,
  RDI = 0x07,
  RBP = 0x05,
  RSP = 0x04,
  R8 = 0x08,
  R9 = 0x09,
  R10 = 0x0A,
  R11 = 0x0B,
  R12 = 0x0C,
  R13 = 0x0D,
  R14 = 0x0E,
  R15 = 0x0F
};
