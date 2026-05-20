all: os-image

os-image: kernel.bin, boot_sect_main.bin
	cat ./bootloader/boot_sect_main.bin ./bin/kernel.bin > ./bin/os-image.bin

kernel.bin: kernel_entry.o kernel.o
	mkdir -p bin
	ld -m elf_i386 -o ./bin/kernel.bin -Ttext 0x1000 ./objs/kernel_entry.o ./objs/kernel.o --oformat binary

kernel_entry.o:
	mkdir -p objs
	nasm -f elf32 ./kernel/kernel_entry.asm -o ./objs/kernel_entry.o

kernel.o: 
	mkdir -p objs
	gcc -m32 -ffreestanding -fno-pie -fno-pic -c ./kernel/kernel.c -o ./objs/kernel.o

boot_sect_main.bin: 
	nasm -f bin ./bootloader/boot_sect.asm -o ./bootloader/boot_sect_main.bin

clear: 
	rm -rf ./bin/* ./objs/*

run: 
	qemu-system-x86_64 -fda ./bin/os-image.bin