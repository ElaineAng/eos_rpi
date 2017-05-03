SHELL = /bin/sh

OS = kernel7
QEMU = qemu-system-arm
KERN_DIR = kernel

GCCPREFIX = 'arm-none-eabi-'

CC = $(GCCPREFIX)gcc 
OBJDUMP = $(GCCPREFIX)objdump
OBJCOPY = $(GCCPREFIX)objcopy

TARGET = $(OS).elf

QEMUOPTS = -kernel $(TARGET) -M raspi2 -m 256 -serial mon:stdio -D qemu.log -nographic

CFLAGS := -O2 -Wall -Wextra -mcpu=cortex-a7 -fpic -ffreestanding -std=gnu99 -I include

OBJS = boot.o $(KERN_DIR)/uart.o $(KERN_DIR)/timer.o $(KERN_DIR)/kernel.o


.PRECIOUS: %.o

all: $(TARGET)

boot.o: boot.S
	$(CC) $(CFLAGS) -c $< -o $@

$(KERN_DIR)/%.o: $(KERN_DIR)%.c 
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS) linker.ld
	$(CC) -T linker.ld -o $@ $(CFLAGS) -nostdlib $(OBJS)

qemu: $(TARGET)
	$(QEMU) $(QEMUOPTS)

img: $(TARGET)
	$(OBJCOPY) $(TARGET) -O binary $(OS).img

gdb:
	arm-none-eabi-gdb

qemu-gdb: $(TARGET)
	$(QEMU) $(QEMUOPTS) -gdb tcp::26000


clean: 
	rm -f *.o *.elf *.img
	rm -f $(KERN_DIR)/*.o
