//  this table is used to define the interrupts for the kernel 
//  final output to load for the BIOS again is just the address and teh size as we did in the GDT
// The IDT is basically an table that the CPU look when an interrupt happens as in to what to do with that?
#pragma once 
#include "types.h"

#define KERNEL_CS 0x08 // why defining segments again, we shiofted to flat memory model right? 
// well, although we are defining the idt in C, but the table still has to be loaded uin assembly in the REAL MODE, so we have to define the segment first and then we will give the handler address so that the CPU can go there to handle the address. 

// this is a hardware defined data structure, nothing to worry about here. 
// this is the struct for each entry in the idt table. gate or handler.
typedef struct {
    u16 low_offset; // Lower 16 bit of the handler address, nwo why do we split it inn lower and higher, well again computers were first 16 bits, so things are made according to them. 
    u16 sel; // the selector segement 
    u8 always0;
    u8 flags; 
    u16 high_offset; // the higher 16 bits 
} __attribute__((packed)) idt_gate_t; // packed?: well that is so that compiler does not add padding to align the memory, as this data is to be set on harware, and those bits are defined as they are, if the computer adds padding there, it will be a mess interpreting those instructions. 

// struct for the definition of the idt table
typedef struct{
    u16 limit; 
    u32 base; 
} __attribute__((packed)) idt_register_t; // register or table, both same thing 

#define IDT_ENTRIES 256 // the CPU accepts 256 entries in the table, each range reserved for specific type of interrupts. 

idt_gate_t idt[IDT_ENTRIES]; 
idt_register_t idt_reg; 

void set_idt_gate(int n, u32 handler); 
void set_idt(); // this function to set the idt table 
