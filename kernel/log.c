
// this uses the console subsystem 
#include <subsystems/console/console.h>

void log_info(const char *module,const char *msg){
    console_write("[INFO] ");
    console_write(module); 
    console_write(" "); 
    console_write(msg);
    console_putc('\n');
}

void log_warn(const char *module,const char *msg){
    console_write("[WARN] ");
    console_write(module);
    console_write(" ");
    console_write(msg);
    console_putc('\n');
}

void log_error(const char *module,const char *msg){
    console_write("[ERROR] ");
    console_write(module);
    console_write(" ");
    console_write(msg);
    console_putc('\n');
}