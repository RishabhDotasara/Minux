#include "isr.h"
#include "idt.h"

void isr_install(){
    set_idt_gate(0, isr0);
    set_idt_gate(1, isr1);
    set_idt_gate(2, isr2);
    set_idt_gate(3, isr3);
    set_idt_gate(4, isr4);
    set_idt_gate(5, isr5);
    set_idt_gate(6, isr6);
    set_idt_gate(7, isr7);
    set_idt_gate(8, isr8);
    set_idt_gate(9, isr9);
    set_idt_gate(10, isr10);
    set_idt_gate(0, isr1);
    set_idt_gate(0, isr1);
    set_idt_gate(0, isr1);
    set_idt_gate(0, isr1);
}