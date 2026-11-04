CROSS   := aarch64-linux-gnu-
CC      := $(CROSS)gcc
OBJDUMP := $(CROSS)objdump
QEMU    := qemu-system-aarch64

CFLAGS  := -Wall -Wextra -Werror -ffreestanding -mgeneral-regs-only -g -O0 -Iinclude
LDFLAGS := -nostdlib -nostartfiles -static -no-pie -Wl,--build-id=none -T linker/linker_script.ld

BUILD   := build
SRCS_C  := $(wildcard src/*.c)
SRCS_S  := $(wildcard src/*.s)
OBJS    := $(patsubst src/%.c,$(BUILD)/%.o,$(SRCS_C)) $(patsubst src/%.s,$(BUILD)/%.o,$(SRCS_S))
TARGET  := $(BUILD)/kernel.elf

QEMU_FLAGS := -machine virt -cpu cortex-a53 -nographic -kernel $(TARGET)

all: $(TARGET)

$(TARGET): $(OBJS) linker/linker_script.ld
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@

$(BUILD)/%.o: src/%.c $(wildcard include/*.h) | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: src/%.s | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $@

run: $(TARGET)
	$(QEMU) $(QEMU_FLAGS)

debug: $(TARGET)
	$(QEMU) $(QEMU_FLAGS) -S -s

gdb: $(TARGET)
	gdb-multiarch $(TARGET) -ex "target remote :1234"

disasm: $(TARGET)
	$(OBJDUMP) -d $(TARGET)

clean:
	rm -rf $(BUILD)

re: clean all

.PHONY: all run debug gdb disasm clean re
