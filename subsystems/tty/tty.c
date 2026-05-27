#include <subsystems/console/console.h>

void tty_clear(){
    console_clear(); 
}

void tty_write(char* s){
    int i = 0; 
    while (s[i] != '\0'){
        console_putc(s[i]);
        i++;
    }
}

void kprint(char *string)
{
    int i = 0;
    while (string[i] != '\0')
    {
        print_char(-1, -1, string[i], 0);
        i++;
    }
}

void tty_putchar(char c){
    print_char(-1, -1, c, 0);
}

void init_tty()
{
    tty_clear();
    console_set_cursor(0, 0);
}
