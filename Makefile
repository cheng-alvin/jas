CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I ../include -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

BUILD = build
HOME = .
DIRS = engine

jas:
	@for dir in $(DIRS); do \ $(MAKE) -C $(HOME)/$$dir; \ done
	$(LD) -r $(wildcard $(HOME)/$(DIRS)/*.o) -o $(BUILD)/$@

clean:
	rm -r -f *.o $(BUILD)
	mkdir -p $(BUILD)