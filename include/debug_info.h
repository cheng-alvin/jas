/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * This struct is used to store the debug information for the instruction.
 * The debug information includes the file, line content, and line number.
 *
 * @note Please see the intel manual for more information on the instruction
 * details and bits.
 *
 * TODO extend debugger struct
 *
 * @param file The file that the instruction is located in.
 * @param line_content The line content of the instruction.
 * @param line_number The line number of the instruction.
 */

typedef struct {
  char *file;
  char *line_content;
  int line_number;
} info_t;