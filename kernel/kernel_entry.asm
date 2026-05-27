; so , why this file?

; now the issue is that , as you can see in the main routine in our bootloader, we simply jump to the address 0x1000 for the kernel code, but that may not be the correct address, because the final binary code is being spit by the compiler, which to optimise performance or for any ranndom reason can rearrange the order of execution in the asm code spat out, so we need to make sure that the main function of the kernel is the one always running while jumping in the kernel code. 

; so, how do we solve that? 
; well, we will create a kernel)_entry routine that will ensure that we will compile in object file and link it with the kernel file, with the entry code coming before the kernel code, and then creating the os-image.bin, and in this way we will have a kernel entry routine before the actuial execution of the kernel starts, so we can directly point the control flow to the kernel entry main function 

[bits 32]
global _start
[extern main]; Declate that we will be referencing the external symbol ’ main ’,
; so the linker can substitute the final address 

_start: 
    call main ; invoke the main function of the kernel.
    jmp $ ; hang the CPU after the return from the main function.