BUILD = build

all: 
	$(MAKE) -C libjas libjas.a

test:
	$(MAKE) -C tests

format:
	@find ./ -type f \( -name "*.cpp" -o -name "*.c" -o -name "*.h" \) -exec clang-format --verbose -i {} +

clean:
	find . -name "*.o" -type f -delete
	find . -name "*.a" -type f -delete
	$(MAKE) -C tests clean
	rm -r -f $(BUILD)
	mkdir -p $(BUILD)

.PHONY: all test format clean
