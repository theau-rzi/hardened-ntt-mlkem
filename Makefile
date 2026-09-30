# Toolchain ARM
CC = arm-none-eabi-gcc
MACH = cortex-m4

# Flags d'architecture et de sécurité
CFLAGS = -c -mcpu=$(MACH) -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Werror -O3 -std=c11
INCLUDES = -I include/

# Flags d'édition de liens (nosys pour simuler l'absence d'OS)
LDFLAGS = -mcpu=$(MACH) -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 --specs=nosys.specs -Wl,-Map=build/ntt_cortex.map

# Fichiers (à adapter quand tu remettras ton fichier C)
SRCS = src/main.c
OBJS = $(SRCS:src/%.c=build/%.o)
TARGET = build/ntt_cortex.elf

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build/

# Lancement dans l'émulateur (mode sans interface)
run: all
	qemu-system-arm -M lm3s6965evb -cpu cortex-m4 -nographic -semihosting -kernel $(TARGET)

.PHONY: all clean run
