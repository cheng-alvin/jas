BUILD = build

all: clean
	$(MAKE) -C libjas tabs.c
	$(MAKE) -C libjas libjas.a -j8
	@find . -name "*.o" -type f -delete
	$(MAKE) -C libjas libjas_debug.a -j8
	cp libjas/include/*.h $(BUILD)/include
	mv $(BUILD)/libjas.a $(BUILD)/lib/libjas.a
	mv $(BUILD)/libjas_debug.a $(BUILD)/lib/libjas_debug.a
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
	@rm -rf libjas/tabs.c

tests: 
	$(MAKE) -C tests

.PHONY: all tests format clean
