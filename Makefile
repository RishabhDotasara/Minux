os-image: kernel.bin 
	cat ./bootloader/boot_sect_main.bin ./kernel.bin > os-image.bin

kernel.bin: kernel_entry.o kernel.o
	ld -m elf_i386 -o ./kernel.bin -Ttext 0x1000 ./objs/kernel_entry.o ./objs/kernel.o --oformat binary

kernel_entry.o:
	mkdir -p objs
	nasm -f elf32 ./kernel/kernel_entry.asm -o ./objs/kernel_entry.o

kernel.o: 
	mkdir -p objs
	gcc -m32 -ffreestanding -fno-pie -fno-pic -c ./kernel/kernel.c -o ./objs/kernel.o