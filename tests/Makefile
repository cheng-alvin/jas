CC = clang

CFLAGS_COMMON = -I ../ -I ../libjas/ -lstdc++ -g
CFLAGS = $(CFLAGS_COMMON)

TESTS = $(patsubst %.c, %, $(wildcard *.c)) 

tests: CFLAGS = $(CFLAGS_COMMON)
tests: $(TESTS)

$(TESTS): libjas_debug.a
$(TESTS): %: %.o 
	$(CC) -o $@ $< ../build/libjas_debug.a $(CFLAGS)

libjas_debug.a: 
	$(MAKE) -C ../libjas libjas_debug.a

clean:
	@find . -name "*.o" -type f -delete
	@find . -name "*.a" -type f -delete
	@rm -r -f $(BUILD)
	@mkdir -p $(BUILD)

.PHONY: clean tests
