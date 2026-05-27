

// screen is a memory mapped IO, so it will be easy to  write the driver, first we will define the constants, all 

#include "vga.h"
#include <arch/x86/cpu/low_level.h>
#include <subsystems/console/console.h>
#include <kernel/log.h>

// private function declarations
int get_screen_offset(int row, int col);
int get_cursor_offset();
void set_cursor_offset(int offset);
void print_char(int row, int col, char character, char attribute);
int get_scroll_offset();
int get_offset_row(int offset); 
int get_offset_col(int offset);

// private function implementations

int get_screen_offset(int row, int col)
{
    return (col + MAX_COLS * row) * 2;
}

int get_cursor_offset()
{
    // use the control register to get the current cursor position 
    // the control register is at port 0x3D4, and the data register is at port 0x3D5 
    // to get the high byte of the cursor position, we need to write 14 to the control register, and then read from the data register 
    // to get the low byte of the cursor position, we need to write 15 to the control register, and then read from the data register
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8; /* High byte: << 8 */
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2; /* Position * size of character cell */
}

void set_cursor_offset(int offset)
{
    offset /= 2; // convert from byte offset to cell offset 

    port_byte_out(REG_SCREEN_CTRL,14);
    port_byte_out(REG_SCREEN_DATA, (offset >> 8) & 0xFF); // high byte
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out( REG_SCREEN_DATA, offset & 0xFF); // low byte
}

void print_char(int row, int col, char character, char attribute)
{
    unsigned char *video_address = (unsigned char *)VIDEO_ADDRESS;

    if (!attribute)
    {
        attribute = WHITE_ON_BLACK; // default style
    }

    // the whole layout here is layed flat, so your finala ddress is going to be col + COL * row
    int offset;
    if (col >= 0 && row >= 0)
    {
        // write to that position
        offset = get_screen_offset(row, col);
    }
    else
    {
        // write to the current cursor position
        offset = get_cursor_offset();
    }

   

    // yes, you have to code every character brother, nothing is already done for you.
    if (character == '\n')
    {
        // set teh offset to the last of the current row 
        offset = get_screen_offset(get_offset_row(offset) + 1, 0); // move to the next line
        // set_cursor_offset(offset)
    }
    else if (character == '\b'){
        kprint_backspace();
        return;
    }
    else
    {
        *(video_address + offset) = character;
        *(video_address + offset + 1) = attribute;
        // after writing , set eh cursor to the next cell
        offset += 2;
    }

    int crow = get_offset_row(offset); 
    int ccol = get_offset_col(offset); 

    // if the final position goes out of the screen, scroll the screen
    if (crow >= MAX_ROWS)
    {
        // copy the memory a row before
        for (int i = 1; i < MAX_ROWS; i++)
        {
            memory_copy(video_address + get_screen_offset(i, 0), video_address + get_screen_offset(i - 1, 0), MAX_COLS * 2);
        }
        // clear the last line
        int last_line_offset = get_screen_offset(MAX_ROWS - 1, 0);
        for (int i = 0; i < MAX_COLS; i++)
        {
            *(video_address + last_line_offset + i * 2) = ' ';
            video_address[last_line_offset + i * 2 + 1] = WHITE_ON_BLACK;
        }

        offset = get_screen_offset(MAX_ROWS - 1, 0);
    }

    set_cursor_offset(offset);
}

int get_offset_row(int offset){
    return offset / (2 * MAX_COLS);
}

int get_offset_col(int offset){
    int row = get_offset_row(offset); 
    return (offset - (row * MAX_COLS*2))/2 ;
}


// console functions implementation 
void vga_putc(console_device_t* dev, char c){
    print_char(-1, -1, c, WHITE_ON_BLACK);
}

void vga_clear(console_device_t* dev){
    int total_cells = dev->height * dev->width; // total must be even
    // set cursor to 0 
    set_cursor_offset(get_screen_offset(0,0));
    u16* screen = (u16*)VIDEO_ADDRESS;
    for (int i = 0; i < total_cells/2; i++){
        //  now just set all chars to " "
        screen[i * 2] = ' ';
        screen[i*2 + 1] = WHITE_ON_BLACK; 
    }
}

void vga_set_cursor(console_device_t* dev, u32 x, u32 y){
    set_cursor_offset(get_screen_offset(x, y)); 
}

// define the vga console ops respectively to be used by the console API
static console_ops_t vga_ops = {
    .putc = &vga_putc,
    .clear = &vga_clear,
    .set_cursor = &vga_set_cursor,
};

struct console_device vga_device = {
    .name = "VGA Driver",
    .width = 80,
    .height = 25,
    .ops = &vga_ops};

void init_vga()
{
    console_register(&vga_device);
    log_info("[VGA]", "VGA Initialised Successfully!\n");
}

// public fucntion implementations
void kprint_backspace(){
    int offset = get_cursor_offset() - 2; 
    int row = get_offset_row(offset); 
    int col = get_offset_col(offset); 

    if (row <= 0 && col <= 0){
        print_char(0, 0, ' ', WHITE_ON_BLACK);
        set_cursor_offset(get_screen_offset(0, 0));
        return;
    }

    print_char(row, col, ' ', WHITE_ON_BLACK);
    set_cursor_offset(offset);
}