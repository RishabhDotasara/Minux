#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "tty.h"
#include "../drivers/keyboard.h"
#include "../drivers/ata.h"
#include "../libc/string.h"
#include "../libc/mem.h"

void welcome(){
    // ascii art
    char line1[] = "           _       _             ___  ____  \n";
    char line2[] = " _ __ ___ (_)_ __ (_) ___  _ __ / _ \\/ ___| \n";
    char line3[] = "| '_ ` _ \\| | '_ \\| |/ _ \\| '_ \\ | | \\___ \\ \n";
    char line4[] = "| | | | | | | | | | | (_) | | | | |_| |___) |\n";
    char line5[] = "|_| |_| |_|_|_| |_|_|\\___/|_| |_|\\___/|____/ \n";
    char newline[] = "\n";

    tty_write("Welcome to MinionOS!\nType HELP to open help menu.\n");
}

void shell(){
    // a little shell program, however it does not have any commands for now, just keeps echoing whatever you type  
    char line[256];
    char buf[1024];
    memory_set(buf, 0, sizeof(buf));

    while(1){
        tty_write("root@minionOS>");

        tty_readline(line, sizeof(line), 1);

        if (strcmp(line, "READ") == 0){
            tty_write("Hello\n");
        }
    }
}

void main() {
    init_tty();
    // setup the idt table
    isr_install();
    irq_install();

    //   initialise all the drivers here 
    init_ata();


    welcome();
    shell();
  
}
