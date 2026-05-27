#include <arch/x86/cpu/types.h>

// this subsystem is to have an interafce for the presistent storage devices 

typedef struct block_device; // to be defined down 

typedef struct block_device_ops{
    // two major functions, read and write 
    void (*read)(
        u32 lba, 
        u32 count,
        char* buffer
    );

    void (*write)(
        u32 lba, 
        u32 count,
        const char* data
    )
} block_device_ops_t;

// for now, just name and ops, in future there could be more attributes 
typedef struct block_device {
    char* name; 
    u32 block_size; 
    u32 block_count;
    block_device_ops_t* ops;
} block_device_t;

void block_read(u32 lba, u32 count, char *buffer);
void register_block(block_device_t *dev);
block_device_t *block_get_active();