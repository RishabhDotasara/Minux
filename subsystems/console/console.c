#include "console.h"

static console_device_t* active_console = 0; 

void console_register(console_device_t* dev){
    active_console = dev; 
}

console_device_t* console_get_active(){
    return active_console;
}

// now implement the generic functions for the console api 
void console_putc(char c){
    // put the character on the active console 
    if (!active_console) return; 

    active_console->ops->putc(active_console, c);
}

void console_clear(){
    if (!active_console) return; 

    active_console->ops->clear(active_console); 
}

void console_set_cursor(u32 x, u32 y){
    active_console->ops->set_cursor(active_console, x, y);
}


