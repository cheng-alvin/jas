#ifndef JAS_INIT_H
#define JAS_INIT_H

#include "error.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Code-generation mode for the assembler module code, can
 * be used as a indicator for the assembler code generator
 * to generate code.
 */

typedef enum {
  JAS_MODE_16,
  JAS_MODE_32,
  JAS_MODE_64,
} jasMode_t;

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Output format for the assembler module code, can be used
 * as a indicator for the assembler code generator to generate
 * code. An elf format is used for the assembler to generate
 * code for the elf file format. A raw format is used for the
 * assembler to generate code for the raw binary file format.
 *
 * Basically means if there's any headers or other stuff that
 * bloat up the file.
 */

typedef enum {
  JAS_OUT_FORMAT_ELF,
  JAS_OUT_FORMAT_RAW,
} jasOutFormat_t;

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Struct for declaring the assembler instance or unit to
 * generate code to. This allows multiple assembler instances
 * to be created and used in the same program. They can also use
 * different code-generation modes to your customization needs!
 */

typedef struct {
  uint8_t *buffer;
  jasMode_t mode;
  int bufferLen;
  jasOutFormat_t format;
  char **includes;
  char *filename;
  bool isFile;
} jasInstance_t;

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Function used to create another assembler translation unit.
 * This allows multiple assembler instances to be created and
 * used in the same program. They can also use different
 * code-generation modes to your customization needs! As mentioned
 * in the `jasInstance_t` struct documentation.
 *
 * @param mode The code-generation mode for the assembler instance.
 * @param instance The pointer to the assembler instance to be created.
 * @param buffer The buffer to be used for the assembler instance.
 *
 * @see `jasInstance_t` and `jasMode_t` for more type information.
 * @note The `instance` parameter can be `NULL` if you want to create
 * a new instance.
 *
 * @returns The status code for the function.
 * @see `jasErrorCode_t` for more information on the status codes.
 *
 * @note Function expects that the pointer is already allocated with
 * `malloc` or `realloc`!
 */

jasErrorCode_t jasInitNew(jasMode_t mode, jasInstance_t *instance, uint8_t *buffer);

#endif
