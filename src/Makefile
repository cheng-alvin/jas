CC = clang
LD = clang
DB = lldb

CFLAGS_COMMON = -I ../include -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

MODULES = codegen

BUILD = ../build
HOME = ..

CODEGEN_OBJ = mov.o
MAIN_OBJ = main.o parse.o endian.o write.o

all: clean jas debug

jas: CFLAGS = $(CFLAGS_COMMON)
jas: modules $(MAIN_OBJ)
	$(LD) $(wordlist 2,$(words $^),$^) -o $(BUILD)/$@ 

debug: CFLAGS = $(CFLAGS_COMMON) -g
debug: clean $(MAIN_OBJ)
	$(LD) $(wordlist 2,$(words $^),$^) -o $(BUILD)/$@ 
	$(DB) $(BUILD)/debug

modules: codegen.o

codegen.o: CFLAGS = $(CFLAGS_COMMON)
codegen.o: $(CODEGEN_OBJ)
	$(LD) $^ -o $@ 
	@cp $@ $(BUILD)/$@

clean:
	rm -r -f *.o $(BUILD)
	mkdir -p $(BUILD)
