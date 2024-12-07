BUILD = build

include libjas/libjas.mk
include tests/tests.mk

all: clean
	$(MAKE) -C libjas libjas.a
	cp libjas/include/*.h $(BUILD)/include
	mv $(BUILD)/libjas.a $(BUILD)/lib/libjas.a
	cp README.md $(BUILD)
	cp LICENSE $(BUILD)

format:
	@find ./ -type f \( -name "*.cpp" -o -name "*.c" -o -name "*.h" \) -exec clang-format --verbose -i {} +

clean:
	@find . -name "*.o" -type f -delete
	@find . -name "*.a" -type f -delete
	@rm -r -f $(BUILD)
	@mkdir -p $(BUILD)/include
	@mkdir -p $(BUILD)/lib
	@mkdir -p $(BUILD)
	@$(MAKE) -C tests clean

tests: 
	$(MAKE) -C tests

.PHONY: all tests format clean
