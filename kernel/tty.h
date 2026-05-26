
// this module here is responsile for creating all the characters that we have in the buffer into meaningful text on the screen, as in interpret all the characters and translate them to terminal outputs as expected

void init_tty();
void clear_screen(); 
void tty_putchar(char c);
void tty_readline(char* buf, int max, int iflag);

void tty_split(char **buf, char *src, char del);
void tty_write(char *string);