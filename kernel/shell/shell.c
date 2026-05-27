#include "shell.h"
#include "../log.h"
#include <lib/string.h>
#include <lib/mem.h>
#include <subsystems/tty/tty.h>


#define SHELL_LOG "[SHELL]"

void init_shell(){
    log_info(SHELL_LOG, "Shell Initialised\n");
    start_shell();
}

void start_shell(){

    while (1){
        // main shell loop 

        tty_write("minux>");
        char line[512]; 
        memory_set(line, 0, 512);
        tty_readline(line); 
        char args[10]; // max 10 args allowed for now. 
        strsplit(line, ' '); 

        tty_write("You wrote: ");
        tty_write(line[0]);
        tty_write("\n");
    }
}
