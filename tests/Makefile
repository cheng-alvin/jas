CC = clang

CFLAGS_COMMON = -I ../libjas/include -lcriterion -lstdc++ -g
CFLAGS = $(CFLAGS_COMMON)

TESTS = buffer endian error operand

all: CFLAGS = $(CFLAGS_COMMON)
all: $(TESTS)
	@for test in $(TESTS); do \
		./$$test; \
	done
	@rm -r $(TESTS)

$(TESTS): libjas_debug.a
$(TESTS): %: %.o 
	$(CC) -o $@ $< ../build/libjas_debug.a $(CFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

libjas_debug.a: 
	$(MAKE) -C ../libjas libjas_debug.a

clean:
	@rm -rf $(TESTS)

.PHONY: all clean