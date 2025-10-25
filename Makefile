BUILD = build
CC = clang

CFLAGS_COMMON = -I libjas/include
CFLAGS = $(CFLAGS_COMMON)

.PHONY: all tests format clean pre_build

all: clean $(BUILD)/lib/libjas.a $(BUILD)/lib/libjas_debug.a

format:
	@find ./ -type f \( -name "*.cpp" -o -name "*.c" -o -name "*.h" \) -exec clang-format --verbose -i {} +

pre_build:
	@find . -name "*.o" -type f -delete

clean:
	@find . -name "*.o" -type f -delete
	@find . -name "*.a" -type f -delete
	@rm -r -f $(BUILD)
	@mkdir -p $(BUILD)/include
	@mkdir -p $(BUILD)/lib
	@mkdir -p $(BUILD)make
	@$(MAKE) -C tests clean
	@rm -rf libjas/instructions.inc
	@cp libjas/include/*.h $(BUILD)/include
	@cp README.md $(BUILD)
	@cp LICENSE $(BUILD)
	@cp THANKS.txt $(BUILD)

tests: 
	$(MAKE) -C tests

$(BUILD)/lib/libjas.a: 
	$(MAKE) pre_build
	$(MAKE) -C libjas instructions.inc
	$(MAKE) -C libjas libjas.a
	mv $(BUILD)/libjas.a $(BUILD)/lib/libjas.a

$(BUILD)/lib/libjas_debug.a: 
	$(MAKE) pre_build
	$(MAKE) -C libjas instructions.inc
	$(MAKE) -C libjas libjas_debug.a
	mv $(BUILD)/libjas_debug.a $(BUILD)/lib/libjas_debug.a
