#include "label.h"
#include "test.h"

Test(label, create) {
  label_create("label", false, false, 0xFFFF, NULL);

  assert_str_eq(
      label_table[0].name, "label",
      "Label name is incorrect.");

  assert(label_table[0].exported == false);
  assert(label_table[0].ext == false);
  assert(label_table[0].address == 0xFFFF);

  label_destroy_all();
}

Test(label, lookup) {
  label_create("label", false, false, 0xFFFF, 0);
  label_t *label = label_lookup("label");

  assert_str_eq(
      label->name, "label",
      "Label name is incorrect.");

  assert(label->exported == false);
  assert(label->ext == false);
  assert(label->address == 0xFFFF);

  label_destroy_all();
}

int main(void) {
  TestSuite(label);

  RunTest(label, create);
  RunTest(label, lookup);

  return 0;
}
