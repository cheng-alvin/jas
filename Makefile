CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

BUILD = build
HOME = .
DIRS = libjas labrador

all: jas

jas: clean
	$(MAKE) -C libjas

format:
	clang-format -i **/*.{c,h} --verbose

clean:
	find . -name "*.o" -type f -delete
	rm -r -f $(BUILD)
	mkdir -p $(BUILD)
	mkdir -p $(BUILD)/test
	mkdir -p $(BUILD)/test/libjas
	find . -name "*.dylib" -type f -delete

test: jas
	$(MAKE) -C test
  