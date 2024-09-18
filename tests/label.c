#include "label.h"
#include "test.h"

Test(label, create) {
  label_create("label", false, false, 0xFFFF);
  label_t *labels = label_dump_all();

  assert_str_eq(
      labels[0].name, "label",
      "Label name is incorrect.");

  assert(labels[0].exported == false);
  assert(labels[0].ext == false);
  assert(labels[0].address == 0xFFFF);

  label_destroy_all();
}

Test(label, lookup) {
  label_create("label", false, false, 0xFFFF);
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
