BUILD = build

all: 
	$(MAKE) -C libjas libjas.a
	mkdir -p $(BUILD)/include
	cp libjas/include/*.h $(BUILD)/include
	cp README.md $(BUILD)
	cp LICENSE $(BUILD)

format:
	@find ./ -type f \( -name "*.cpp" -o -name "*.c" -o -name "*.h" \) -exec clang-format --verbose -i {} +

clean:
	@find . -name "*.o" -type f -delete
	@find . -name "*.a" -type f -delete
	@rm -r -f $(BUILD)
	@mkdir -p $(BUILD)

tests: 
	$(MAKE) -C tests

.PHONY: all tests format clean
