CC = clang
LD = clang
DB = lldb

CFLAGS_COMMON = -I ../include -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

OBJ = main.o parse.o endian.o write.o
BUILD = ../build

all: clean jas debug

jas: CFLAGS = $(CFLAGS_COMMON)
jas: $(OBJ)
	$(LD) $^ -o $(BUILD)/$@ 

debug: CFLAGS = $(CFLAGS_COMMON) -g
debug: clean $(OBJ)
	$(LD) $(wordlist 2,$(words $^),$^) -o $(BUILD)/$@ 
	$(DB) $(BUILD)/debug

clean:
	rm -r -f *.o $(BUILD)
	mkdir -p $(BUILD)
