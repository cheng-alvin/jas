CC = clang
LD = ld
DB = lldb

CFLAGS_COMMON = -I ../libjas/include -I ../libjas -I ../labrador -Wall -Wextra
CFLAGS = $(CFLAGS_COMMON)

BUILD = ../build
HOME = ../

EXE = parse.o

all: libjas.o libjas

libjas.o:
	$(MAKE) -C ../libjas

libjas: CFLAGS = $(CFLAGS_COMMON)
libjas: $(EXE)
	@clear
	@for exe in $(EXE); do \
		$(CC) $$exe $(BUILD)/libjas.o -o $(BUILD)/test/libjas/$${exe%.o}; \
		cd $(BUILD)/test/libjas/; \
		echo "\033[1mTesting \`$${exe%.o}\` ...\033[0m \c"; \
		./$${exe%.o}; \
		if [ 0 ]; then \
			echo "\033[1m\033[32msuccess!\033[0m"; \
		fi; \
		done
			

	