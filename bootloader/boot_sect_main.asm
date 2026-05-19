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

    mov bx, HELO_WORLD ; 
    call print_string 
    call print_nl
    call switch_to_pm
    
[bits 32]
start_protected_code: 
    mov ebx, R2P_Transition
    call print_string_pm

jmp $ ; the reason to use this infinite loop here actually quite funny, see if you dont put it, assembly nature to execute the instructions line by line will execute those functions once again, so either have some instruction to halt the CPU there or for now simply just run this infinite loop 

[bits 16]
; include all the files 
%include "boot_sect_print_str.asm"
%include "boot_sect_print_hex.asm"
%include "boot_sect_print_nl.asm"
%include "boot_sect_disk.asm"
%include "boot_sect_gdt.asm"
%include "boot_sect_print_32bit.asm"
%include "boot_sect_r2p.asm"
HELO_WORLD: 
    db 'Bootsector Loaded Sucessfully!',0

R2P_Transition: 
    db 'Transitioning to Protected Mode...',0

BOOT_DRIVE: 
    db 0

times 510-($-$$) db 0 ; this for now to fill the remaining space from the 512 bytes 

; now place the signature bytes in the file 
db 0x55; this marks this as a bootable bootsector 
db 0xAA
