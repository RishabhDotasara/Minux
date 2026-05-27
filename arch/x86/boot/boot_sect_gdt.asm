; in this file we will define the gdt 
; we wil use the flat simple model of memory here, which just assumes that thememory is a continuous block of 4 GB, and we will define the code segment and data segment to cover the entire memory, this is a common setup for simple protected mode kernels

gdt_start: ; we define this label to mark the start address of the gdt 

gdt_null:
; at the start of each gdt we define a null descriptor, this is a mandatory entry in the gdt, it is used toi return the null response, that if we get a null selector, that gives the response that the memory does not exist, remember '0' means a null here! so to cover the edge case is this used.  
    dd 0x00 
    dd 0x00 
    ; 8 bytes is the size for the null descriptor , so twice definition 

gdt_code: ; the code segment of the gdt
    dw 0xffff; limit 
    dw 0x0 ; base
    db 0x0 ; base (bits 16-23)
    db 10011010b ; 1st flags 
    db 11001111b ; 2nd flags 
    db 0x0 
    ; actually this random order of definition is just a standard, so dont get confused, read the book again maybe fro more clarity.

    
gdt_data: 
    dw 0xffff 
    dw 0x0 
    db 0x0 
    db 10010010b ; 1st flags, the only difference between code and data segment is the executable bit, which is 1 for code and 0 for data
    db 11001111b ; 2nd flags, this is the same for both code and data segment, it defines the granularity and other stuff, we will not go into details
    db 0x0 

gdt_end: 

gdt_descriptor: ; now  we define the gdt descriptor which will be used to load this gdt table 
    dw gdt_end - gdt_start - 1; size of the gdt 
    dd gdt_start ; start of our GDT 
    
; we just define some constants to make it easier to use the gdt, these are the offsets of the code and data segment in the gdt, we will use these offsets to load the segments later in our code.
CODE_SEG equ gdt_code - gdt_start 
DATA_SEG equ gdt_data - gdt_start 