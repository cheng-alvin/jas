CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I ../include -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../build
HOME = ..


MAIN_OBJ = aaa.o aas.o endian.o error.o hint.o parse.o write.o

all: jas.o

jas.o: CFLAGS = $(CFLAGS_COMMON)
jas.o: clean $(MAIN_OBJ)
	$(LD) -r $(wordlist 2,$(words $^),$^) -o $(BUILD)/$@ 

clean:
	rm -r -f *.o $(BUILD)
	mkdir -p $(BUILD)
