; this file to create functions to read from disk using the BIOS services 
; datasheet to setup the registers for the BIOS call: 
; ah -> 0x02 ; read mode 
; al -> number of sectors to read
; ch -> cylinder number
; cl -> sector number
; dh -> head number
; dl -> drive number (0x00 for floppy, 0x80 for hard disk)
; es:bx -> buffer to store the read data


disk_load: 
    ; save the registers 
    pusha 
    push dx 
    ; setup the registers for the BIOS call 

    mov ah, 0x02 ; read sector fucntion 
    mov al, dh 
    mov ch, 0x00 ; cylinder 0
    mov cl, 0x02 ; the next sector after the boot sector (sector numbers start at 1)
    mov dh, 0x00 ; head 0
    ; dl should already be set by the caller
    
    ; raise the interrrupt 
    int 0x13 

    ; error checking 
    jc disk_error ; if the carry flag is set, there was an error during the disk read

    ; check sector count and verify if required number is read from the disk
    pop dx 
    cmp al, dh
    jne sectors_error 
    popa 
    ret 

disk_error: 
    mov bx, DISK_ERROR 
    call print_string 
    call print_nl 
    mov dh, ah ; ah contains the error code
    xor bh, bh ; clear bh to ensure bx only has error code in bl
    mov bl, dh
    call print_hex 
    jmp disk_loop

sectors_error: 
    mov bx, SECTOR_ERROR
    call print_string 
    call print_nl 
    jmp disk_loop



disk_loop: 
    jmp $ 

DISK_ERROR: 
    db 'Disk read error', 0 

SECTOR_ERROR: 
    db 'Sector read error', 0 
