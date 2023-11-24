CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I ../../util/include -I ../include -I .. -I ../../labrador -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../../build
HOME = ../../

OBJ = parse.o 

all: as

as: CFLAGS = $(CFLAGS_COMMON)
as: $(OBJ)
	$(CC) $^ -o $(BUILD)/test/$@ 