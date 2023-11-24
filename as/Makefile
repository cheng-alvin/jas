CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I ../util/include -I include -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../build
HOME = ..

OBJ = parse.o

all: as.o

as.o: CFLAGS = $(CFLAGS_COMMON)
as.o: $(OBJ)
	$(LD) -r $^ -o $(BUILD)/$@ 