# Source files
KERNEL_CSOURCES := $(wildcard ./kernel/*.c)
DRIVER_CSOURCES := $(wildcard ./drivers/*.c)
CPU_CSOURCES := $(wildcard ./cpu/*.c)
LIBC_CSOURCES := $(wildcard ./libc/*.c)

# Object files
KERNEL_COBJS := $(patsubst ./kernel/%.c,./objs/%.o,$(KERNEL_CSOURCES))
DRIVER_COBJS := $(patsubst ./drivers/%.c,./objs/%.o,$(DRIVER_CSOURCES))
CPU_COBJS := $(patsubst ./cpu/%.c,./objs/%.o,$(CPU_CSOURCES))
LIBC_COBJS := $(patsubst ./libc/%.c,./objs/%.o,$(LIBC_CSOURCES))

# Final object list
OBJS := ./objs/kernel_entry.o ./objs/interrupt.o $(KERNEL_COBJS) $(DRIVER_COBJS) $(CPU_COBJS) $(LIBC_COBJS)

DISK_SECTORS := 2048
KERNEL_SECTOR_OFFSET := 1

all: os-image

os-image: kernel.bin boot_sect_main.bin
	mkdir -p bin
	dd if=/dev/zero of=./bin/os-image.bin bs=512 count=$(DISK_SECTORS)
	dd if=./bootloader/boot_sect_main.bin \
	   of=./bin/os-image.bin \
	   conv=notrunc
	dd if=./bin/kernel.bin \
	   of=./bin/os-image.bin \
	   bs=512 seek=$(KERNEL_SECTOR_OFFSET) conv=notrunc

kernel.bin: $(OBJS)
	mkdir -p bin
	ld -m elf_i386 -o ./bin/kernel.bin -Ttext 0x1000 $(OBJS) --oformat binary

# Assemble kernel entry
./objs/kernel_entry.o: ./kernel/kernel_entry.asm
	mkdir -p objs
	nasm -f elf32 ./kernel/kernel_entry.asm -o ./objs/kernel_entry.o

./objs/interrupt.o: ./cpu/interrupt.asm
	mkdir -p objs
	nasm -f elf32 ./cpu/interrupt.asm -o ./objs/interrupt.o

./objs/%.o: ./kernel/%.c
	mkdir -p objs
	gcc -m32 -ffreestanding -fno-pie -fno-pic -c $< -o $@

./objs/%.o: ./drivers/%.c
	mkdir -p objs
	gcc -m32 -ffreestanding -fno-pie -fno-pic -c $< -o $@

./objs/%.o: ./cpu/%.c
	mkdir -p objs
	gcc -m32 -ffreestanding -fno-pie -fno-pic -c $< -o $@

./objs/%.o: ./libc/%.c
	mkdir -p objs
	gcc -m32 -ffreestanding -fno-pie -fno-pic -c $< -o $@

# Build bootloader
boot_sect_main.bin:
	cd bootloader && nasm -f bin boot_sect_main.asm -o boot_sect_main.bin

run: 
	qemu-system-x86_64 -drive format=raw,file=./bin/os-image.bin

write-test-sector: os-image
	echo "HELLODISK" > test.bin
	dd if=test.bin \
	   of=./bin/os-image.bin \
	   bs=512 seek=100 conv=notrunc

clear:
	rm -rf ./bin/* ./objs/* ./bootloader/*.bin test.bin

.PHONY: all run clear write-test-sector