# Host
CC = gcc

# Flags 
CFLAGS = -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Werror -O3 -std=c11
INCLUDES = -I src/core -I src/math

SRCS = src/main.c src/core/subroutines.c src/core/tables.c src/math/ntt.c
OBJS = $(SRCS:src/%.c=build/%.o)
TARGET = build/ntt_host.elf

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

build/%.o: src/%.c | build
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build/

# Lancement natif direct
run: all
	./$(TARGET)

.PHONY: all clean run
