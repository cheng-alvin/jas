#include "error.h"
#include <criterion/criterion.h>

void test_callback(const char *msg) {
  cr_assert_str_eq(msg, "msg", "Error callback function was not called correctly.");
}

Test(error, add_callback) {
  err_add_callback(test_callback);

  cr_assert_not_null(err_callback);
  cr_assert(err_callback == test_callback);
}

Test(error, err) {
  err_add_callback(test_callback);
  err("msg");
}