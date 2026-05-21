#include "idt.h"

void set_idt_gate(int n, u32 handler){
    // this takes the gate number and the handler address to set in the table; 
    idt[n].low_offset = low_16(handler); 
    idt[n].sel = KERNEL_CS; 
    idt[n].always0 = 0x0; 
    idt[n].flags = 0x0E; 
    idt[n].high_offset = high_16(handler);
}

void set_idt(){
    idt_reg.base = (u32) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_register_t) - 1; 
    
    //now load the idt 
    __asm__ __volatile__("lidt (%0)": : "r" (&idt_reg)); // volatile is a way to tell the compiler that the value for the , if you read this code somewhere in future, you might want to search this syntax, not hard though.
}