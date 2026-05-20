

// screen is a memory mapped IO, so it will be easy to  write the driver, first we will define the constants, all 

#include "screen.h"
#include "low_level.h"

int get_screen_offset(int row, int col){
    return (col + MAX_COLS * row)*2;
}


void print_char(int row, int col, char character, char attribute){
    unsigned char* video_address = (unsigned char*) VIDEO_ADDRESS; 

    if (!attribute){
        attribute = WHITE_ON_BLACK; // default style 
    }

    // the whole layout here is layed flat, so your finala ddress is going to be col + COL * row  
    int offset; 
    if (col >= 0 && row >= 0){
        // write to that position 
        offset = get_screen_offset(row, col);
    }
    // else {
    //     // write to the current cursor position
    //     offset = get_cursor(); 
    // }

    // yes, you have to code every character brother, nothing is already done for you. 
    if (character == '\n'){
        // set teh offset to the last of teh current row 
        offset = get_screen_offset(row+1, 0);
    }
    else 
    {
        *(video_address+offset) = character; 
        *(video_address + offset + 1) = attribute; 
    }

    // after writing , set eh cursor to the next cell 
    // set_cursor(video_address+offset+2);
}

void print_string(int row, int col, char *string, char attribute)
{
    int i = 0;

    while (string[i] != '\0')
    {
        print_char(row, col + i, string[i], attribute);
        i++;
    }
}

void clear_screen(){
    // pt empty space at all cells 
    for (int row = 0; row < MAX_ROWS; row++){
        for (int col = 0;  col < MAX_COLS; col++){
            print_char(row, col, ' ', 0);
        }
    }
}