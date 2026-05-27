
 ;print function: to print strings to screen
 ; save the initial pointer in a register and then keep incrementing unless you reach the null byte 
print_string:  
    pusha 


print_loop: 
    mov al, [bx]
    cmp al, 0 ; null character check 
    je end 

    ; if nto equaal , just print the byte 
    mov ah, 0x0E
    int 0x10 

    add bx, 1 
    jmp print_loop 


print_char: 
    mov ah, 0x0E 
    int 0x10 
    ret 

end: 
    popa 
    ret 
