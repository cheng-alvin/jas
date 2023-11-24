CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I ../include -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

BUILD = build
HOME = .
DIRS = as labrador util

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
