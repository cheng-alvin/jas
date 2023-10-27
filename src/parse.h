typedef struct {
  char *instruction;
  char *arg1;
  char *arg2;
  char *arg3;
} instruction_t;

instruction_t **parse(char *buffer);
