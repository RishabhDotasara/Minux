#include <arch/x86/interrupts/isr.h>
#include <drivers/input/keyboard/keyboard.h>
#include <drivers/video/vga/vga.h>
#include <lib/string.h>
#include <subsystems/input/input.h>
#include <kernel/log.h>

#define KBD_LOG "[KBD]"

#define ENTER 0x1c
#define BACKSPACE 0x0E
#define CAPS 0x3A // press event for caps
#define SHIFT_PRESS_LEFT 0x2A
#define SHIFT_RELEASE_LEFT 0xAA 
#define SHIFT_PRESS_RIGHT 0x36 
#define SHIFT_RELEASE_RIGHT 0xB6
#define SPACE 0x39
#define SC_MAX 128

static int capslock_enaled = 0; 
static int shift_enabled = 0;

const char sc_ascii_lower[SC_MAX] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.'};

const char sc_ascii_caps[SC_MAX] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', 0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', 0, '\\',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0, '*', 0, ' ', 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.'};

const char sc_ascii_shift[SC_MAX] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.'};

const char sc_ascii_shift_caps[SC_MAX] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '"', '~', 0, '|',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', 0, '*', 0, ' ', 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.'};

static void keyboard_callback(registers_t regs)
{
  /* The PIC leaves us the scancode in port 0x60 */
  u8 scancode = port_byte_in(0x60);

  // check for special keys
  if (scancode == CAPS)
  {
    capslock_enaled = !capslock_enaled;
    return;
  } // toggle capslock
  if (scancode == SHIFT_PRESS_LEFT || scancode == SHIFT_PRESS_RIGHT)
  {
    shift_enabled = 1;
    return;
  }
  if (scancode == SHIFT_RELEASE_LEFT || scancode == SHIFT_RELEASE_RIGHT)
  {
    shift_enabled = 0;
    return;
  }

  // ignore all release events 
  if (scancode & 0x80)
    return;

  char* table_to_be_used = sc_ascii_lower;


  // now dynamically change the table to be used
  if (shift_enabled && capslock_enaled){
    table_to_be_used = sc_ascii_shift_caps; 
  }
  else if (capslock_enaled){
    table_to_be_used = sc_ascii_caps;
  }
  else if (shift_enabled)
  {
    table_to_be_used = sc_ascii_shift;
  }
  else 
  {
    table_to_be_used = sc_ascii_lower;
  }


  // store in the kernel input buffer
  char c = table_to_be_used[scancode];
  if (c){ // push only printable characters
    input_push(c);
  }
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback);
    log_info(KBD_LOG, "PS/2 keyboard driver initialised");
}
