#pragma once
#include "arch/x86/cpu/types.h"

typedef struct console_device; // device to be used for the ops

// these console ops are what will be used by the subsystems to control the screen without worrying about the driver being used under the hood.
// this will be implemented by the driver , and the function pointers will be used to refer those implementations.
typedef struct console_ops
{
    // three functs, putchar, clearscreen, and set_cursor
    // the cursor management and scrolling part must be implemented at the driver level as per this spec of the console API.
    void (*putc)(struct console_device *dev, char c);

    void (*clear)(struct console_device *dev);

    void (*set_cursor)(struct console_device *dev, u32 x, u32 y);

} console_ops_t;

// now define the console device
typedef struct console_device
{
    const char *name;

    u32 width;
    u32 height;

    void *private_data;

    console_ops_t *ops;

} console_device_t;

// public API, all these functions work on the activel registered console.
void console_register(console_device_t *dev);
console_device_t *console_get_active();
void console_putc(char c); 
void console_write(char* s);
void console_clear();
void console_set_cursor(u32 x, u32 y);