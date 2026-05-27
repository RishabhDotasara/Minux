; print routine for protected mode, in 32 bit 
; here instead of calling interrupts from BIOS, we directly edit the Video memory to print to the screen, this is because in protected mode we dont have access to the BIOS interrupts, and also this is a good practice to learn how to directly interact with the hardware without relying on the BIOS

[bits 32]; telling assembler to emit 32 bit instructions 

VIDEO_MEMORY equ 0xb8000 ; this is the starting address of the video memory in text mode, each character on the screen is represented by 2 bytes in this memory, one for the ASCII code of the character and one for the attribute (color etc.)
WHITE_ON_BLACK equ 0x0f; this one constant to define color  

print_string_pm: 
    pusha ; save the state to avoid corruption 
    mov edx, VIDEO_MEMORY ; we will use edx to point to the video memory we are writing at  

print_string_pm_loop: 
    ; assuming the string starting is at ebx 
    mov al, [ebx] 
    mov ah, WHITE_ON_BLACK 

    cmp al, 0 ; check end of string 
    je pm_end 

    ; now comes printing, here it does not work like putting the value in the register and then flushing it, here we just write the value to the video memroy directly! 
    mov [edx], ax 
    
    ;now just increment both the character and the video memory 
    add edx, 2
    add ebx, 1  

    jmp print_string_pm_loop 
    


pm_end: 
    popa
    ret 
