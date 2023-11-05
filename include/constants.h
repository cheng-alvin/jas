typedef enum {
  LEGACY,
  PROTECTED,
  LONG,
} modes_t;

typedef struct {
  char *file;
  int line_number;
  modes_t mode;
  char *line;
} info_t;