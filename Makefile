BUILD = build
CC = clang

CFLAGS_COMMON = -I libjas/include
CFLAGS = $(CFLAGS_COMMON)

SRC_FILES = $(shell find . -type f \( -name "*.c" -o -name "*.h" \))
OBJ_FILES = $(shell find . -type f -name "*.o")

.PHONY: all tests format clean pre_build format_md

all: clean $(BUILD)/lib/libjas.a $(BUILD)/lib/libjas_debug.a

format_md: $(shell find . -name "*.md") 
	node ./mdformatwrapper.js $^

format: format_md
	@clang-format -i --verbose $(SRC_FILES)

pre_build:
	@rm -rf $(BUILD) $(OBJ_FILES)

clean: pre_build
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
