/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * This enum is used to specify the mode of the program.
 * A mode inside the x86 architecture is used to specify
 * the size of the operands and other information such as inter-
 * rrupts and words.
 *
 * @note Please see the intel manual for more information on the modes.
 *
 * @enum LEGACY The legacy mode is used to specify the 16-bit mode.
 * @enum PROTECTED The protected mode is used to specify the 32-bit mode.
 * @enum LONG The long mode is used to specify the 64-bit mode.
 */

typedef enum {
  LEGACY,
  PROTECTED,
  LONG,
} modes_t;