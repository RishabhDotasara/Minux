
# Source files
KERNEL_CSOURCES := $(wildcard ./kernel/*.c)
DRIVER_CSOURCES := $(wildcard ./drivers/*.c)

# Object files
KERNEL_COBJS := $(patsubst ./kernel/%.c,./objs/%.o,$(KERNEL_CSOURCES))
DRIVER_COBJS := $(patsubst ./drivers/%.c,./objs/%.o,$(DRIVER_CSOURCES))

# Final object list
OBJS := ./objs/kernel_entry.o $(KERNEL_COBJS) $(DRIVER_COBJS)


all: os-image

os-image: kernel.bin boot_sect_main.bin
	mkdir -p bin
	cat ./bootloader/boot_sect_main.bin ./bin/kernel.bin > ./bin/os-image.bin

kernel.bin: $(OBJS)
	mkdir -p bin
	ld -m elf_i386 -o ./bin/kernel.bin -Ttext 0x1000 $(OBJS) --oformat binary

# Assemble kernel entry
./objs/kernel_entry.o: ./kernel/kernel_entry.asm
	mkdir -p objs
	nasm -f elf32 ./kernel/kernel_entry.asm -o ./objs/kernel_entry.o


./objs/%.o: ./kernel/%.c
	mkdir -p objs
	gcc -m32 -ffreestanding -fno-pie -fno-pic -c $< -o $@

./objs/%.o: ./drivers/%.c
	mkdir -p objs
	gcc -m32 -ffreestanding -fno-pie -fno-pic -c $< -o $@


# Build bootloader
boot_sect_main.bin:
	cd bootloader && nasm -f bin boot_sect_main.asm -o boot_sect_main.bin

run: os-image
	qemu-system-x86_64 -fda ./bin/os-image.bin

clear:
	rm -rf ./bin/* ./objs/* ./bootloader/*.bin

.PHONY: all run clear