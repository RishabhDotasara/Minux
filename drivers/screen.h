#pragma once
#define VIDEO_ADDRESS 0xb8000 
#define MAX_ROWS 25 
#define MAX_COLS 80 

#define WHITE_ON_BLACK 0x0f 

// we need ports to control the cursor , essential part of the screen 
#define REG_SCREEN_CTRL 0x3D4 
#define REG_SCREEN_DATA 0x3D5 

void kprint_backspace();
void print_char(int row, int col, char character, char attribute);
