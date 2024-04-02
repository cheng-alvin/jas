#include "error.h"
#include "null.h"

err_callback_t err_callback = NULL;

void err_add_callback(err_callback_t input) {
  err_callback = input;
}

void err(const char *msg) {
  if (err_callback) {
    err_callback(msg);
  }
}