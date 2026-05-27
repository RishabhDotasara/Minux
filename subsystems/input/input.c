#include "input.h"
#include <arch/x86/cpu/types.h>
#include <drivers/input/keyboard/keyboard.h>

#define INP_BUFF_SIZE 256

char inp_buffer[INP_BUFF_SIZE]; // buffer to store the input characters

u32 head = 0; 
u32 tail = 0; 

// write at tail, and read from head 

// public functions  
// a blocking function that waits until there is input available in the buffer and then returns the character
char input_getc(){
    while(input_available() == 0);  
    head = (head+1) % INP_BUFF_SIZE;
    char chr = inp_buffer[head];
    return chr;
}

void input_push(char chr)
{
    u32 next = (tail + 1) % INP_BUFF_SIZE;

    // if tail comes from behind head after rotation ? we cannot add any more characters now!
    if (next != head)
    {
        inp_buffer[next] = chr;
        tail = next;
    }
}

int input_available(){
    return head!=tail;
}


char* get_inp_buffer(){
    return inp_buffer;
}

void init_input(){
    init_keyboard();
}