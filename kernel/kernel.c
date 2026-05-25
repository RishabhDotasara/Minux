#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../libc/string.h"

void welcome(){
    // ascii art
    char line1[] = "           _       _             ___  ____  ";
    char line2[] = " _ __ ___ (_)_ __ (_) ___  _ __ / _ \\/ ___| ";
    char line3[] = "| '_ ` _ \\| | '_ \\| |/ _ \\| '_ \\ | | \\___ \\ ";
    char line4[] = "| | | | | | | | | | | (_) | | | | |_| |___) |";
    char line5[] = "|_| |_| |_|_|_| |_|_|\\___/|_| |_|\\___/|____/ \n";
    char newline[] = "\n";

    print_string(1, 0, line1, 0x0E);
    print_string(2, 0, line2, 0x0E);
    print_string(3, 0, line3, 0x0E);
    print_string(4, 0, line4, 0x0E);
    print_string(5, 0, line5, 0x0E);
}

void main() {
  clear_screen();

  welcome();
  
  // setup the idt table
  isr_install();
  irq_install();
}

void user_input(char* input){
    // if (strcmp(input, "END" == 0)){
    //     kprint("Stopping the CPU. Bye\n"); 
    //     asm volatile("hlt");
    // }

    // for now, simply print what the user has entered 
    kprint("\n$root>");
    kprint("You said:");
    kprint(input);
    kprint("\n");
}