CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

BUILD = build
HOME = .
DIRS = libjas labrador

all: jas

jas:
	@for dir in $(DIRS); do \
		if [ "$$dir" = "labrador" ]; then \
			continue; \
		fi \
		$(MAKE) -C $$dir; \
	done

	$(CC) -o $(BUILD)/$@ $(BUILD)/*.o

clean:
	find . -name "*.o" -type f -delete
	rm -r -f $(BUILD)
	mkdir -p $(BUILD)
	mkdir -p $(BUILD)/test
	mkdir -p $(BUILD)/test/libjas
	find . -name "*.dylib" -type f -delete