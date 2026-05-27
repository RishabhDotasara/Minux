# Compiler flags
CFLAGS := -m32 -ffreestanding -fno-pie -fno-pic -I.

# Source files
KERNEL_CSOURCES := $(wildcard ./kernel/*.c) $(wildcard ./kernel/shell/*.c)
SUBSYSTEM_CSOURCES := $(wildcard ./subsystems/input/*.c) $(wildcard ./subsystems/tty/*.c) $(wildcard ./subsystems/console/*.c) $(wildcard ./subsystems/block/*.c)
DRIVER_CSOURCES := $(wildcard ./drivers/block/ata/*.c) $(wildcard ./drivers/input/keyboard/*.c) $(wildcard ./drivers/video/vga/*.c)
ARCH_CSOURCES := $(wildcard ./arch/x86/cpu/*.c) $(wildcard ./arch/x86/interrupts/*.c) $(wildcard ./arch/x86/timer/*.c)
LIB_CSOURCES := $(wildcard ./lib/*.c)

# Object files (flatten all into build/)
KERNEL_COBJS := $(addprefix ./build/,$(notdir $(KERNEL_CSOURCES:.c=.o)))
SUBSYSTEM_COBJS := $(addprefix ./build/,$(notdir $(SUBSYSTEM_CSOURCES:.c=.o)))
DRIVER_COBJS := $(addprefix ./build/,$(notdir $(DRIVER_CSOURCES:.c=.o)))
ARCH_COBJS := $(addprefix ./build/,$(notdir $(ARCH_CSOURCES:.c=.o)))
LIB_COBJS := $(addprefix ./build/,$(notdir $(LIB_CSOURCES:.c=.o)))

# Final object list
OBJS := ./build/kernel_entry.o ./build/interrupt.o $(KERNEL_COBJS) $(SUBSYSTEM_COBJS) $(DRIVER_COBJS) $(ARCH_COBJS) $(LIB_COBJS)

DISK_SECTORS := 2048
KERNEL_SECTOR_OFFSET := 1

all: os-image

os-image: kernel.bin boot_sect_main.bin
	mkdir -p build
	dd if=/dev/zero of=./build/os-image.bin bs=512 count=$(DISK_SECTORS)
	dd if=./arch/x86/boot/boot_sect_main.bin \
	   of=./build/os-image.bin \
	   conv=notrunc
	dd if=./build/kernel.bin \
	   of=./build/os-image.bin \
	   bs=512 seek=$(KERNEL_SECTOR_OFFSET) conv=notrunc

kernel.bin: $(OBJS)
	mkdir -p build
	ld -m elf_i386 -o ./build/kernel.bin -Ttext 0x1000 $(OBJS) --oformat binary

# Assemble kernel entry
./build/kernel_entry.o: ./kernel/kernel_entry.asm
	mkdir -p build
	nasm -f elf32 ./kernel/kernel_entry.asm -o ./build/kernel_entry.o

./build/interrupt.o: ./arch/x86/interrupts/interrupt.asm
	mkdir -p build
	nasm -f elf32 ./arch/x86/interrupts/interrupt.asm -o ./build/interrupt.o

# Kernel C files
./build/%.o: ./kernel/%.c
	mkdir -p build
	gcc $(CFLAGS) -c $< -o $@

./build/%.o: ./kernel/shell/%.c
	mkdir -p build
	gcc $(CFLAGS) -c $< -o $@

# Subsystem C files
./build/%.o: ./subsystems/input/%.c
	mkdir -p build
	gcc $(CFLAGS) -c $< -o $@

./build/%.o: ./subsystems/tty/%.c
	mkdir -p build
	gcc $(CFLAGS) -c $< -o $@

./build/%.o: ./subsystems/console/%.c
	mkdir -p build
	gcc $(CFLAGS) -c $< -o $@

./build/%.o: ./subsystems/block/%.c
	mkdir -p build
	gcc $(CFLAGS) -c $< -o $@

# Driver C files
./build/%.o: ./drivers/block/ata/%.c
	mkdir -p build
	gcc $(CFLAGS) -c $< -o $@

./build/%.o: ./drivers/input/keyboard/%.c
	mkdir -p build
	gcc $(CFLAGS) -c $< -o $@

./build/%.o: ./drivers/video/vga/%.c
	mkdir -p build
	gcc $(CFLAGS) -c $< -o $@

# Arch C files
./build/%.o: ./arch/x86/cpu/%.c
	mkdir -p build
	gcc $(CFLAGS) -c $< -o $@

./build/%.o: ./arch/x86/interrupts/%.c
	mkdir -p build
	gcc $(CFLAGS) -c $< -o $@

./build/%.o: ./arch/x86/timer/%.c
	mkdir -p build
	gcc $(CFLAGS) -c $< -o $@

# Lib C files
./build/%.o: ./lib/%.c
	mkdir -p build
	gcc $(CFLAGS) -c $< -o $@

# Build bootloader
boot_sect_main.bin:
	cd arch/x86/boot && nasm -f bin boot_sect_main.asm -o boot_sect_main.bin

run: 
	qemu-system-x86_64 -drive format=raw,file=./build/os-image.bin

write-test-sector: os-image
	echo "HELLODISK" > test.bin
	dd if=test.bin \
	   of=./build/os-image.bin \
	   bs=512 seek=100 conv=notrunc

clear:
	rm -rf ./build/* ./arch/x86/boot/*.bin test.bin

.PHONY: all run clear write-test-sector