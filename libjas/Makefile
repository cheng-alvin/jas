CC = clang
LD = ld
DB = lldb

# TODO Update makefile

CFLAGS_COMMON = -I include  -I ./ -Wall -Wextra 
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../build
HOME = ..

OBJ =  aaa.o aad.o aam.o aas.o adc.o add.o and.o bound.o bsf.o bsr.o bswap.o bt.o btc.o btr.o bts.o call.o cbw.o cdqe.o clc.o cld.o clflush.o cli.o clts.o cmc.o codegen.o cwde.o endian.o error.o i.o init.o instruction.o mi.o mode.o modrm.o mr.o operand.o parse.o remove.o rex.o rm.o write.o

all: libjas.o

libjas.o: CFLAGS = $(CFLAGS_COMMON)
libjas.o: $(OBJ)
	$(LD) -r $^ -o $(BUILD)/$@ 

# Please set `DYLD_LIBRARY_PATH` for dynamic library to work as well!

libjas.dylib: CFLAGS = $(CFLAGS_COMMON)
libjas.dylib: $(OBJ)
	$(CC) $^ -o $(BUILD)/$@ -dynamiclib  

# Debug symbol build, please only use for debugging purposes!
# (Basically adds lots of junk into your object file)

debug: CFLAGS = $(CFLAGS_COMMON) -g
debug: $(OBJ)
	$(LD) -r $^ -o $(BUILD)/$@