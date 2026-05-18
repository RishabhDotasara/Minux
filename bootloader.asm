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

    mov al, 'H'
    call print
    mov al, 'E'
    call print   
    mov al, 'L'
    call print 
    call print 
    mov al, 'O'    
    call print
    mov dx, 0x0A0A
    call print_hex


















jmp $ ; the reason to use this infinite loop here actually quite funny, see if you dont put it, assembly nature to execute the instructions line by line will execute those functions once again, so either have some instruction to halt the CPU there or for now simply just run this infinite loop 

 ;print function: to print string characters to screen 
print: 
    mov ah, 0x0E; set ttp mode 
    int 0x10; call BIOS video interrupt, assuming the character is loaded in the al register 
    ret; we use this so that the program uses stack to keep track of the trace 

; function to print hex values 
; the strategy to print hex values is simple, just loop over each hex value, convert that to ascii and then we just print it. Simple right?
; we use the cx register as counter in x86 generally, and the dx register for the hex value to print 

print_hex: 
    mov cx, 0
    call hex_loop

hex_loop: 
    ; since our registers here are 16 bits, we have to loop twice to print the 
    cmp cx, 4
    je end
    mov ax, dx 
    and ax, 0xf000
    shr ax,12
    add al, 0x30 ;to shife the digit to ascii region of the numerical and alphabetic region to compare and check for numeric or alphabet accordingly, al here cause we use the lower bits only, little endian system 
    cmp al, 0x39 
    jle step2 ; if less means numeriv value, we simply print it  
    add al, 7 ; as A starts at 65, so instead of 30, we will add 37 to get to A 
    jmp step2

step2:
    ; to run if the value is numeric, simply put it on the register and print it 
    call print 
    shl dx, 4
    inc cx 
    jmp hex_loop

end: 
    ret

times 510-($-$$) db 0 ; this for now to fill the remaining space from the 512 bytes 

; now place the signature bytes in the file 
db 0x55; this marks this as a bootable bootsector 
db 0xAA
