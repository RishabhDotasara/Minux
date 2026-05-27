#include <arch/x86/interrupts/isr.h>
#include <arch/x86/timer/timer.h>
#include <subsystems/tty/tty.h>
#include <subsystems/input/input.h>
#include <drivers/video/vga/vga.h>
#include <lib/string.h>
#include <lib/mem.h>

#define MAX_ARGS 10 

void init_arch(){
    isr_install(); 
    irq_install(); 
}

void init_drivers(){
    init_vga(); 
}

void init_subsystems(){
    init_tty(); 
    init_input();
}

void main() {
    init_arch(); 

    init_drivers(); // we have all hardwares abstracted by now to use in our subsystems  

    init_subsystems(); // initialise all the subsystems now 

    // now you can use the subsystems,
    // tty_clear(); 
    tty_write("[KERNEL] Kernel Initialised!\n");

    // just take the input and put it to the screen 
    while (1){
        char c = input_getc(); 
        tty_write(&c); 
    }
}

