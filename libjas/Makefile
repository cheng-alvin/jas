CC = clang
LD = ld
DB = lldb

# TODO Update makefile

CFLAGS_COMMON = -I include  -I ./ -Wall -Wextra 
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../build
HOME = ..

OBJ = bsr.o cwde.o  mr.o aaa.o bswap.o endian.o operand.o aad.o bt.o  error.o parse.o aam.o btc.o i.o aas.o btr.o remove.o adc.o bts.o init.o add.o instruction.o rex.o and.o cbw.o rm.o bound.o cdqe.o mi.o write.o bsf.o codegen.o modrm.o     

# Note, call.o has been removed because it's stashed for now.
# Please add in later. 

all: libjas.o

libjas.o: CFLAGS = $(CFLAGS_COMMON)
libjas.o: $(OBJ)
	$(LD) -r $^ -o $(BUILD)/$@ 

# Please set `DYLD_LIBRARY_PATH` for dynamic library to work as well!

libjas.dylib: CFLAGS = $(CFLAGS_COMMON)
libjas.dylib: $(OBJ)
	$(CC) $^ -o $(BUILD)/$@ -dynamiclib  