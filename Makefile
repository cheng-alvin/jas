CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I ../include -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

BUILD = build
HOME = .
DIRS = engine labrador util

all: jas

jas:
	@for dir in $(DIRS); do \
		$(MAKE) -C $$dir; \
	done

	$(CC) -o $(BUILD)/$@ $(BUILD)/*.o

test:
	$(MAKE) -C test
	@for dir in $(DIRS); do \
		$(MAKE) test -C $$dir; \
	done

	$(CC) -o $(BUILD)/$@ $(BUILD)/*.o -g	


clean:
	rm -r -f **/*.o $(BUILD)
	mkdir -p $(BUILD)
