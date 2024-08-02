#include "error.h"
#include "test.h"

void test_callback(const char *msg) {
  assert_str_eq(msg, "msg", "Error callback function was not called correctly.");
}

Test(error, add_callback) {
  err_add_callback(test_callback);

  assert_not_null(err_callback);
  assert(err_callback == test_callback);
}

Test(error, err) {
  err_add_callback(test_callback);
  err("msg");
}

int main(void) {
  TestSuite(error);

  RunTest(error, add_callback);
  RunTest(error, err);

  return 0;
}