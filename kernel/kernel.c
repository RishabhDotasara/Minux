#include <arch/x86/interrupts/isr.h>
#include <arch/x86/timer/timer.h>
#include <subsystems/tty/tty.h>
#include <subsystems/input/input.h>
#include <subsystems/block/block.h>
#include <drivers/video/vga/vga.h>
#include <drivers/block/ata/ata.h>
#include <lib/string.h>
#include <lib/mem.h>
#include <kernel/shell/shell.h>

#define MAX_ARGS 10 

void init_arch(){
    isr_install(); 
    irq_install(); 
}

void init_drivers(){
    init_vga(); 
    init_ata();
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
    tty_write(" __  __ _                   \n");
    tty_write("|  \\/  (_)_ __  _   ___  __\n");
    tty_write("| |\\/| | | '_ \\| | | \\ \\/ /\n");
    tty_write("| |  | | | | | | |_| |>  < \n");
    tty_write("|_|  |_|_|_| |_|\\__,_/_/\\_\\\n");

    tty_write("\n");
    tty_write("    Minimal Unix-like eXperiment\n");
    tty_write("\n");

    tty_write("----------------------------------------\n");
    tty_write(" [OK] x86 Protected Mode\n");
    tty_write(" [OK] Interrupt Handlers\n");
    tty_write(" [OK] PIT Timer\n");
    tty_write(" [OK] VGA Console\n");
    tty_write(" [OK] PS/2 Keyboard\n");
    tty_write(" [OK] ATA Driver\n");
    tty_write("----------------------------------------\n");


    tty_write("minux> ");
    // just take the input and put it to the screen 
    
    // read from the 100th sectro to check the subsystem is working or not 
    // char buff[512] = "Hello WOrld!, written by the kernel to disk! Can you believe it is working?\n";
    // block_write(100, 1, buff);
    // memory_set(buff, 0, 512); // reset to check if we read the data.
    // block_read(100, 1, buff);
    // tty_write(buff);
    init_shell();


}

