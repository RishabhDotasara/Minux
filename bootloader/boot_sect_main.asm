; set the offset address to 0x7C00 
; this, so that all the address from now onward refer to this as the segment for address calculation 
org 0x7C00
bits 16 ; emit 16 bit instructions, 'cause we are still in the Real Mode, adn registers can also store only 16 bits in a x86 CPU

; setup the stack, else we will rely on whatever we get from the firware, but stack correctness is important for  call and ret 

KERNEL_OFFSET equ 0x1000 ; this is the offset where we will load our kernel, to be passed in the disk read routine


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
    
    ; before switching to protected mode, we wil load the kernel, and then switch to the 32 bit mode execute its instructions, as compilers spit out 32 bit instructions, to know more, read the book again that we concatenate the boot sector and kernel code into a kernel image, so it is easier for us to load the kernel
    call load_kernel
    
    mov bx, R2P_Transition
    call print_string
    call print_nl 
    
    call switch_to_pm ; jumps to a label init_pm in the boot_sect_r2p.asm file, where we will switch to protected mode and then execute the kernel code there
    
[bits 32]
init_pm: 
    mov ax, DATA_SEG ; update the segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000 ; update the stack right at the top of the free space
    mov esp, ebp

    ; jump to the loaded kernel code now 
    mov eax, KERNEL_OFFSET 
    jmp eax 


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

[bits 16]
load_kernel: 
    ; set the parameters for disk read 
    mov bx, MSG_LOAD_KERNEL
    call print_string
    call print_nl 

    mov bx, KERNEL_OFFSET ; offset where we want to load the kernel 
    mov dh, 32 ; kernel is ~8KB+, need enough sectors (32 * 512 = 16KB)
    mov dl, [BOOT_DRIVE] 
    call disk_load 

    mov bx, MSG_LOAD_KERNEL_SUCCESS
    call print_string
    call print_nl

    ret



HELO_WORLD: 
    db 'Bootsector Loaded Sucessfully!',0

MSG_LOAD_KERNEL: 
    db 'Loading Kernel...',0

MSG_LOAD_KERNEL_SUCCESS:
    db 'Kernel Loaded Successfully!',0


BOOT_DRIVE: 
    db 0

R2P_Transition: 
    db 'Transitioning to Protected Mode...',0

times 510-($-$$) db 0 ; this for now to fill the remaining space from the 512 bytes 

; now place the signature bytes in the file 
db 0x55; this marks this as a bootable bootsector 
db 0xAA
