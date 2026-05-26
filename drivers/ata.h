#include "../cpu/types.h"

#define PRIMARY_BASE_PORT 0x1F0  
#define SECONDARY_BASE_PORT 0x170 
#define FLOATING_BUS_BYTE 0xFF
#define INDENTIFY_COMMAND 0xEC
#define ATA_SR_ERR 0x01 
#define ATA_SR_BSY 0x80 
#define ATA_SR_DRQ 0x08

struct DRIVE_OFFSETS{
    u8 data_port; 
    u8 error_register; 
    u8 sector_count_register; 
    u8 LBAlo; 
    u8 LBAmid; 
    u8 LBAhi;
    u8 drive_register;
    u8 command_register;
    u8 status_register; 
};

// default offsets for primary/secondary drives (as byte offsets from base port) 
static struct DRIVE_OFFSETS drive_offsets = {
    .data_port = 0,
    .error_register = 1,
    .sector_count_register = 2,
    .LBAlo = 3,
    .LBAmid = 4,
    .LBAhi = 5,
    .drive_register = 6,
    .command_register = 7,
    .status_register = 7
};

// the command ports in read mode act as the status ports for the disk 
int identify_drive(); // this function to check if there are any disks connected to the bus, if not the status register shows the FLOATING_BUS_BYTE 

void init_ata(); // main function where all the work of checking the drives will happen

int ata_read_sectors(u32 LBA, u32 sector_count, char *buf);