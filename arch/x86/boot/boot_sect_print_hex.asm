


; function to print hex values 
; the strategy to print hex values is simple, just loop over each hex value, convert that to ascii and then we just print it. Simple right?
; we use the cx register as counter in x86 generally, and the bx register for the hex value to print 
print_hex: 
    pusha
    mov cx, 0
    
hex_loop: 
    ; since our registers here are 16 bits, we have to loop twice to print the 
    cmp cx, 4
    je stop
    mov ax, bx 
    and ax, 0xf000
    shr ax,12
    add al, 0x30 ;to shife the digit to ascii region of the numerical and alphabetic region to compare and check for numeric or alphabet accordingly, al here cause we use the lower bits only, little endian system 
    cmp al, 0x39 
    jle step2 ; if less means numeriv value, we simply print it  
    add al, 7 ; as A starts at 65, so instead of 30, we will add 37 to get to A 

step2:
    ; to run if the value is numeric, simply put it on the register and print it 
    call print_char
    shl bx, 4
    inc cx 
    jmp hex_loop

stop: 
    popa
    ret
