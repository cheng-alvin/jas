CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I include -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../build
HOME = ..


OBJ = endian.o error.o init.o parse.o write.o

all: engine.o 

engine.o: CFLAGS = $(CFLAGS_COMMON)
engine.o: $(OBJ)
	$(LD) -r $(wordlist 2,$(words $^),$^) -o $(BUILD)/$@ 