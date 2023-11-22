CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I include -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../build
HOME = ..

OBJ = endian.o init.o error.o write.o 
TEST = main.o

all: util.o 

test: CFLAGS = $(CFLAGS_COMMON) -g
test: $(OBJ) $(foreach test,$(TEST),test/$(test))
	$(CC) $^ -o $(BUILD)/test/util 
	@clear
	@$(BUILD)/test/util


util.o: CFLAGS = $(CFLAGS_COMMON)
util.o: $(OBJ) 
	$(LD) -r $^ -o $(BUILD)/$@ 
