# Main Makefile of the Jas assembler, the central process that 
# orchestrates the compilation of associated project files across
# the entire repository.

# Usage: $ make all         # Builds everything

# Miscellaneous targets such as `format`, `pre_build` and `clean`
# are not to be confused as compilation targets as it does not
# build any real code as indicated by the `.PHONY` flag.

# - `clean` resets the entire working directory's build artifacts 
#			and re-instantiates informational files in the `build`

# - `pre_build` Should be executed after every compilation of any
#               of Jas' internal libraries to ensure the currency
#               for project compiler outcomes.


CC = clang

BUILD = build
LIB = $(BUILD)/lib

MD_FILES    := $(shell find . -name "*.md")
C_SRC 	    := $(shell find libjas -name '*.c')
H_SRC       := $(shell find libjas -name '*.h')

SRC_FILES 	:= $(C_SRC) $(H_SRC)

.PHONY: all tests format clean pre_build 
all: clean $(BUILD)/lib/libjas.a $(BUILD)/lib/libjas_debug.a

format:
	@clang-format -i --verbose $(SRC_FILES)
	@node ./mdformatwrapper.js $(MD_FILES)

$(LIB)/libjas%.a: pre_build
	$(MAKE) -C libjas instructions.inc 
	$(MAKE) -C libjas $@ BUILD=$(abspath $(LIB))

pre_build: $(shell find . -type f -name "*.o")
	@rm -rf $(BUILD) $^

clean: pre_build
	@mkdir -p $(BUILD)/include $(LIB)
	@$(MAKE) -C tests clean
	@rm -rf libjas/instructions.inc
	@cp libjas/include/*.h $(BUILD)/include
	@cp README.md LICENSE THANKS.txt $(BUILD)

# test:
# Currently not extensively used - unit testing would
# be reworked ~one day~ when unit testing is required
