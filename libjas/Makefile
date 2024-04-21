CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I include  -I ./ -Wall -Wextra 
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../build
HOME = ..

OBJ = buffer.o instruction.o map.o # Add source files here 

all: libjas.o

libjas.o: CFLAGS = $(CFLAGS_COMMON) -Ofast
libjas.o: $(OBJ)
	$(LD) -r $^ -o $(BUILD)/$@ 

# Please set `DYLD_LIBRARY_PATH` for dynamic library to work as well!

libjas.dylib: CFLAGS = $(CFLAGS_COMMON) -Ofast
libjas.dylib: $(OBJ)
	$(CC) $^ -o $(BUILD)/$@ -dynamiclib  

# Debug symbol build, please only use for debugging purposes!
# (Basically adds lots of junk into your object file)

debug: CFLAGS = $(CFLAGS_COMMON) -g -O0
debug: $(OBJ)
	$(LD) -r $^ -o $(BUILD)/$@