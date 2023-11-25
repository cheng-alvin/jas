CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I ../libjas/include -I ../libjas -I ../labrador -Wall -Wextra -E
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../build
HOME = ../

EXE = _parse.o
DEPS = libjas.o

all: libjas

libjas.o:
	$(MAKE) -C ../libjas

libjas: CFLAGS = $(CFLAGS_COMMON)
libjas: $(DEPS) $(EXE)
	@clear
	@for exe in $(EXE); do \
		echo "Linking: $$exe"; \
		$(CC) $$exe $(BUILD)/libjas.o -o $(BUILD)/test/libjas/$${exe%.o}; \
		echo "Testing: $$exe"; \
		cd $(BUILD)/test/libjas/; \
		pwd; \
		./$${exe%.o}; \
	done
	