#include "../drivers/screen.h"
#include "input.h"
#include "../libc/mem.h"
#include "../libc/string.h"


void clear_screen()
{
    // Write directly to video memory instead of calling print_char per cell,
    // which would trigger 2000 cursor updates via port I/O and cause flickering.
    unsigned char *video_address = (unsigned char *)VIDEO_ADDRESS;
    int total = MAX_ROWS * MAX_COLS;
    for (int i = 0; i < total; i++)
    {
        video_address[i * 2] = ' ';
        video_address[i * 2 + 1] = WHITE_ON_BLACK;
    }
    set_cursor_offset(get_screen_offset(0, 0));
}

void kprint(char *string)
{
    int i = 0;
    while (string[i] != '\0')
    {
        print_char(-1, -1, string[i], WHITE_ON_BLACK);
        i++;
    }
}

void tty_putchar(char c){
    print_char(-1, -1, c, WHITE_ON_BLACK);
}

tty_write(char* string){
    kprint(string);
}

tty_readline(char* buf, int max, int iflag){

    // this is a blocking call, waits until a line is finished. 
    int i = 0;
    while (1){
        char c = kgetchar(); 

        if (c == '\n'){
            tty_putchar('\n');
            buf[i] = '\0'; // terminate the string 
            break;
        }
        else if(c == '\b'){
            if (iflag)kprint_backspace();
            i--;
            continue;
        }
        else 
        {
            if (i < max - 1){
                buf[i++] = c;
                if (iflag) tty_putchar(c);
            }
        }
    }


}
void tty_split(char **buf, char *src, char del)
{
    int i = 0;
    int j = 0;

    // Skip any leading delimiters
    while (src[j] == del)
    {
        j++;
    }

    // Set first argument pointer
    if (src[j] != '\0')
    {
        buf[i++] = &src[j];
    }

    while (src[j] != '\0')
    {
        if (src[j] == del)
        {
            src[j] = '\0'; // Replace delimiter with null-terminator
            j++;

            // Skip contiguous delimiters
            while (src[j] == del)
            {
                j++;
            }

            // Set the next argument pointer
            if (src[j] != '\0')
            {
                buf[i++] = &src[j];
            }
        }
        else
        {
            j++;
        }
    }

    buf[i] = 0; // Null-terminate the argument array
}

void init_tty()
{
    clear_screen();
}
