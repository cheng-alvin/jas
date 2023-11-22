CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I ../include -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

BUILD = build
HOME = .
DIRS = engine test util

jas:
	@for dir in $(DIRS); do \
		$(MAKE) -C $$dir; \
	done

	$(CC) -o $(BUILD)/$@ $(BUILD)/*.o

clean:
	rm -r -f **/*.o $(BUILD)
	mkdir -p $(BUILD)
