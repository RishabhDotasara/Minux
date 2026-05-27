#include <subsystems/console/console.h>
#include <subsystems/input/input.h>
#include <kernel/log.h>

#define TTY_LOG "[TTY]"

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

char tty_readc(){
    char c = input_getc();
    // print the character whn you read it 
    tty_putchar(c);
    return c; 
}

void tty_readline(char* buf){
    // read char untl a n is encountered 
    int i = 0;
    while (1){
        char c = tty_readc(); 
        if (c == '\n') break;

        buf[i] = c;
        i++;
    }
}

void init_tty()
{
    tty_clear();
    console_set_cursor(0, 0);
    log_info(TTY_LOG, "TTY subsystem initialised");
}
