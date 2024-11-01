CC = clang

CFLAGS_COMMON = -I ../libjas/include -I ../libjas/ -lstdc++ -g
CFLAGS = $(CFLAGS_COMMON)

TESTS = $(patsubst %.c, %, $(wildcard *.c)) 

all: CFLAGS = $(CFLAGS_COMMON)
all: $(TESTS)

$(TESTS): libjas_debug.a
$(TESTS): %: %.o 
	$(CC) -o $@ $< ../build/libjas_debug.a $(CFLAGS)

libjas_debug.a: 
	$(MAKE) -C ../libjas libjas_debug.a

clean:
	@find . -name "*.o" -type f -delete
	@find . -name "*.a" -type f -delete
	@find . -type f -perm +111 -exec rm -f {} \;

.PHONY: clean all
