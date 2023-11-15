CC = clang
LD = clang
DB = lldb

CFLAGS_COMMON = -I ../include -Wall -Wextra -c
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../build
HOME = ..


MAIN_OBJ = aaa.o endian.o error.o hint.o parse.o write.o

all: jas

jas: CFLAGS = $(CFLAGS_COMMON)
jas: clean $(MAIN_OBJ)
	$(LD) $(wordlist 2,$(words $^),$^) -o $(BUILD)/$@ 

clean:
	rm -r -f *.o $(BUILD)
	mkdir -p $(BUILD)
