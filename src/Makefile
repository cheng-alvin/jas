CC = clang
LD = clang
DB = lldb

CFLAGS_COMMON = -I ../include -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../build
HOME = ..


MAIN_OBJ = main.o parse.o endian.o write.o

all: jas debug

jas: CFLAGS = $(CFLAGS_COMMON)
jas: clean $(MAIN_OBJ)
	$(LD) $(wordlist 2,$(words $^),$^) -o $(BUILD)/$@ 

debug: CFLAGS = $(CFLAGS_COMMON) -g
debug: modules clean $(MAIN_OBJ)
	$(LD) $(wordlist 3,$(words $^),$^) -o $(BUILD)/$@ 
	$(DB) $(BUILD)/debug

clean:
	rm -r -f *.o $(BUILD)
	mkdir -p $(BUILD)
