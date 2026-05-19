; set the offset address to 0x7C00 
; this, so that all the address from now onward refer to this as the segment for address calculation 
org 0x7C00
bits 16 ; emit 16 bit instructions, 'cause we are still in the Real Mode, adn registers can also store only 16 bits in a x86 CPU

; setup the stack, else we will rely on whatever we get from the firware, but stack correctness is important for  call and ret 

; disable interrupts for init of stack 
cli 
xor ax, ax 
mov ss, ax 
mov es, ax 
mov ds, ax 
mov bp, 0x7C00
mov sp, bp
sti ; enable interrupts after stack is setup

start: 
    mov [BOOT_DRIVE], dl ; save the boot drive number provided by BIOS

    mov bx, HELO_WORLD 
    call print_string
    call print_nl

    ; read from the disk 
    mov bx, 0x9000 ; the address where the read data will be stored by the disk call function 
    mov dh, 0x01 ; number of sectors to read
    mov dl, [BOOT_DRIVE]
    call disk_load 

    mov bx, 0x9000 ; Ensure bx points to the data we just read
    call print_string


jmp $ ; the reason to use this infinite loop here actually quite funny, see if you dont put it, assembly nature to execute the instructions line by line will execute those functions once again, so either have some instruction to halt the CPU there or for now simply just run this infinite loop 

; include all the files 
%include "boot_sect_print_str.asm"
%include "boot_sect_print_hex.asm"
%include "boot_sect_print_nl.asm"
%include "boot_sect_disk.asm"

HELO_WORLD: 
    db 'Hello World!',0

GOODBYE_WORLD:
    db 'Successfully read from sector 2!', 0

BOOT_DRIVE: 
    db 0

times 510-($-$$) db 0 ; this for now to fill the remaining space from the 512 bytes 

; now place the signature bytes in the file 
db 0x55; this marks this as a bootable bootsector 
db 0xAA

; This data starts in Sector 2 (at offset 512 from start of file)
DATA_FROM_DISK:
    db 'Successfully read from sector 2!', 0

times 512 - ($ - DATA_FROM_DISK) db 0 ; fill rest of sector 2
times 512 db 0 ; sector 3
 