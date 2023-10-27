typedef struct {
  char *instruction;
  char *arg1;
  char *arg2;
} instruction_t;

char **parse(char *buffer);
// instruction_t **parse(char *buffer);
