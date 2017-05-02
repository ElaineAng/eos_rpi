SHELL = /bin/sh

OS = kernel7
QEMU = qemu-system-arm
GCCPREFIX = 'arm-none-eabi-'

CC = $(GCCPREFIX)gcc 
OBJDUMP = $(GCCPREFIX)objdump
OBJCOPY = $(GCCPREFIX)objcopy

TARGET = $(OS).elf

# GDBPORT	:= $(shell expr `id -u` % 5000 + 24000)
QEMUOPTS = -kernel $(TARGET) -M raspi2 -m 256 -serial mon:stdio -D qemu.log -nographic

CFLAGS := -O2 -Wall -Wextra -mcpu=cortex-a7 -fpic -ffreestanding -std=gnu99

.PRECIOUS: %.o

all: $(TARGET)

boot.o: boot.S
	$(CC) $(CFLAGS) -c $< -o $@

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): boot.o kernel.o linker.ld
	$(CC) -T linker.ld -o $@ $(CFLAGS) -nostdlib boot.o kernel.o

qemu: $(TARGET)
	$(QEMU) $(QEMUOPTS)

img: $(TARGET)
	$(OBJCOPY) $(TARGET) -O binary $(OS).img

gdb:
	arm-none-eabi-gdb

qemu-gdb: $(TARGET)
	$(QEMU) $(QEMUOPTS) -gdb tcp::26000


clean: 
	rm -f *.o *.elf *.img *.out
