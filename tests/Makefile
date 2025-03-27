CC = clang

CFLAGS_COMMON = -I ../libjas/include -I ../libjas/ -lstdc++ -g -O0 -fsanitize=address 
CFLAGS = $(CFLAGS_COMMON)

TESTS = $(patsubst %.c, %, $(wildcard *.c)) 

all: CFLAGS = $(CFLAGS_COMMON)
all: $(TESTS)

$(TESTS): libjas_debug.a
$(TESTS): %: %.o 
	$(CC) -o $@ $< ../build/libjas_debug.a $(CFLAGS)

libjas_debug.a: 
	$(MAKE) -C ../libjas tabs.c
	$(MAKE) -C ../libjas libjas_debug.a

clean:
	@find . -name "*.o" -type f -delete
	@find . -name "*.a" -type f -delete
	@rm -rf $(TESTS)

.PHONY: clean all
