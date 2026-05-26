#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "tty.h"
#include "../drivers/keyboard.h"
#include "../drivers/ata.h"
#include "../libc/string.h"
#include "../libc/mem.h"

#define MAX_ARGS 10 

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
        
        char* args[MAX_ARGS]; 
        // init all args as null or 0 for bool checks at least 
        for (int i = 0; i < MAX_ARGS; i++){
            *(args[i]) = "\0";
        }


        tty_split(args, line, ' ');
        
        if (strcmp(args[0] ,"READ") == 0){
            int LBA = kstoi(args[1]) ;
            int nos = kstoi(args[2]) ; 

            if (strcmp(args[1], "-H") == 0){
                // print the help menu
                tty_write("READ <LBA> <sector_count> - Read sectors from disk\n");
                tty_write("  LBA: Logical Block Address (default: 100)\n");
                tty_write("  sector_count: Number of sectors to read (default: 1)\n");
                tty_write("  Example: READ 0 5\n");
                tty_write("READ -H - Display this help menu\n");
                continue;
            }

            // issue the read command 
            char* bufff[512 * 5];
            ata_read_sectors(LBA, nos, bufff);
            tty_write(bufff);
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
