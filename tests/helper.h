#ifndef TESTS_HELPER_H
#define TESTS_HELPER_H

#define RUN_TEST_SUITE(suite_name)                             \
  int main(void) {                                             \
    int number_failed;                                         \
    Suite *s;                                                  \
    SRunner *sr;                                               \
    s = suite_name();                                          \
    sr = srunner_create(s);                                    \
    srunner_run_all(sr, CK_NORMAL);                            \
    number_failed = srunner_ntests_failed(sr);                 \
    srunner_free(sr);                                          \
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE; \
  }

#endif