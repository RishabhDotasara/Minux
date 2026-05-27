#include "block.h"

static block_device_t *active_device = 0;

void register_block(block_device_t *dev) { active_device = dev; }
block_device_t *block_get_active() { return active_device; }

void block_read(u32 lba, u32 count, char *buffer)
{
    active_device->ops->read(lba, count, buffer);
}

void block_write(u32 lba, u32 count, char *data)
{
    active_device->ops->write(lba, count, data);
}