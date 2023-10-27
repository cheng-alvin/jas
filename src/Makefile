CC = gcc
LD = gcc
DB = gdb

OBJ = main.o parse.o
BUILD = ../build

all: clean jas

jas: $(OBJ)
	$(LD) $^ -o $(BUILD)/$@ 

CFLAGS = -g

debug: $(OBJ)
	$(LD) $^ -o $(BUILD)/$@ $(CFLAGS) 
	sudo $(DB) $(BUILD)/$@

clean:
	rm -r -f *.o $(BUILD)
	mkdir -p $(BUILD)