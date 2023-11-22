CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I ../util/include -I include -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../build
HOME = ..

OBJ = parse.o

all: engine.o 

engine.o: CFLAGS = $(CFLAGS_COMMON)
engine.o: $(OBJ)
	$(LD) -r $(wordlist 2,$(words $^),$^) -o $(BUILD)/$@ 