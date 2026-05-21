#include "../drivers/screen.h"

void main()
{
    clear_screen();

    // ascii art 
    char line1[] = "  __  __                     ___  _____ ";
    char line2[] = " |  \\/  | ___  _____      __/ _ \\/ ____|";
    char line3[] = " | \\  / |/ _ \\/ _ \\ \\ /\\ / / | | \\___ \\ ";
    char line4[] = " | |\\/| |  __/ (_) \\ V  V /| |_| |___) |";
    char line5[] = " |_|  |_|\\___|\\___/ \\_/\\_/  \\___/|____/ \n";
    char newline[] = "\n";
    
    print_string(1, 0, line1, 0x0B); 
    print_string(2, 0, line2, 0x0B);
    print_string(3, 0, line3, 0x0B);
    print_string(4, 0, line4, 0x0B);
    print_string(5, 0, line5, 0x0B);
    // printf(newline, WHITE_ON_BLACK);
    char str[] = "[INFO] Kernel Loaded Successfully!\n";
    for (int i = 0; i < 30; i++){
        printf(str, 0x0F);
    }
    // printf(str, 0x0F);
    // printf(str, 0x0F);
    // printf(str, 0x0F);
    // printf(str, 0x0F);

    // print_string(MAX_ROWS+10, 0, str, WHITE_ON_BLACK);
    // printf(str, 0x0f);

    // main kernel fucntion starts here 

}