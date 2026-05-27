#include "ata.h"
#include <arch/x86/cpu/low_level.h>
#include <arch/x86/cpu/types.h>
#include <arch/x86/interrupts/isr.h>
#include <subsystems/block/block.h>
#include <kernel/log.h>

#define ATA_LOG "[ATA]"


block_device_ops_t ata_ops = {
    .read = &ata_read_sectors,
    .write = &ata_write_sectors
};

block_device_t ata_driver = {
    .name = "ATA Driver", 
    .block_size = 512, 
    .block_count = 10000,
    .ops = &ata_ops 
};

int identify_drive(u32 base_port){
    // u32 primary_drive_status = port_byte_in(PRIMARY_COMMAND_PORT);
    // u32 secondary_drives_status = port_byte_in(SECONDARY_COMMAND_PORT); 

    // if (primary_drive_status == FLOATING_BUS_BYTE & secondary_drives_status == FLOATING_BUS_BYTE){
    //     tty_write("[DISK-DRIVER]: No Drives Found Connected!\n");
    //     return 0;
    // }

    // IDENTIFY commadn to rescue for definitive drive existence test
    // anything here is directly from the oswiki, go read there. ATA PIO MODE
    port_byte_out(base_port + drive_offsets.drive_register, 0xA0);
    port_byte_out(base_port + drive_offsets.LBAlo, 0);
    port_byte_out(base_port + drive_offsets.LBAmid, 0);
    port_byte_out(base_port + drive_offsets.LBAhi, 0);
    port_byte_out(base_port + drive_offsets.command_register, INDENTIFY_COMMAND);
    // read from the status register
    u8 status_primary = port_byte_in(base_port + drive_offsets.status_register);

    if (status_primary == 0){
        log_warn(ATA_LOG, "Drive not found on this port");
        return 0;
    }

    while(1){
        // wait for bsy to clear
        status_primary = port_byte_in(base_port + drive_offsets.status_register);

        if (status_primary & ATA_SR_ERR){
            // error bit set 
            // tty_write("[DISK] Error identifying the Disk!\n");
            return 0;
        }

        if (!(status_primary & ATA_SR_BSY) && (status_primary & ATA_SR_DRQ)){
            // drive is ready with the data — we MUST drain the 256 words
            // of identify data from the data port, even if we don't use them.
            // Skipping this leaves the drive in a dirty state and corrupts
            // subsequent read commands.
            for (int i = 0; i < 256; i++){
                port_word_in(base_port);
            }   
            log_info(ATA_LOG, "Drive identified successfully");
            break; 
        }
    }
    return 1; 
}

int ata_read_sectors(u32 LBA, u32 sector_count, char* buf){
    // we are going to implement 28 bit PIO here. ie the number of sectors we can address are going to be 2^28 - 1,. which is around 137 GB, considering each sector as 512 bytes.  
    // drive_type: 0 for master and 1 for slave
    // following the os dev wiki page again. 
    // this function just issues the request, that will be fulfilled some time in the future. async loading of data via interrupts.

    // check the sector_number for the max in 28 bit PIO 
    if (LBA > 0x0FFFFFFF) { 
        log_error(ATA_LOG, "LBA out of range for 28-bit PIO");
        return 0;
    }

    if (sector_count == 0) return 0;

    // select the drive for operation, this is the 6th register that does so, read in the theory.
    while (port_byte_in(0x1F7) & ATA_SR_BSY);
    port_byte_out(0x1F6, 0xE0 |(LBA >> 24 & 0x0000000F));
    // 400 ns wait 
    port_byte_in(0x1F7);
    port_byte_in(0x1F7);
    port_byte_in(0x1F7);
    port_byte_in(0x1F7);

    // now this sequence is common for both drives 
    // sector count 
    port_byte_out(0x1F2, sector_count); 

    // send the LBA in 3 different ports 
    port_byte_out(0x1F3, (LBA & 0x000000FF)); // low 8 bits
    port_byte_out(0x1F4, (LBA >> 8) & 0x000000FF); //mid 8 bits
    port_byte_out(0x1F5, (LBA >> 16) & 0x000000FF); // higher 8 bits
    // this completed the 28 bit LBA 

    

    // read command issue 
    port_byte_out(0x1F7, 0x20); 

    // we use polling for now for collecting data.
    u8 status_primary;
    int current_sector = 0; 

    for(int sector = 0; sector < sector_count; sector++)
    {
        while (1)
        {
            status_primary =
                port_byte_in(PRIMARY_BASE_PORT +
                             drive_offsets.status_register);

            if (status_primary & ATA_SR_ERR)
            {
                log_error(ATA_LOG, "Error reading sector from disk");
                return 0;
            }

            if (!(status_primary & ATA_SR_BSY) &&
                (status_primary & ATA_SR_DRQ))
            {
                break;
            }
        }

        // drive is ready with the data, you can read now the identify data , 512 bytes prepared by the drive, which can be read from the port in 512/2 port_out operations
        for (int i = 0; i < 256; i++){
            ((u16*)buf)[sector * 256 + i] = port_word_in(PRIMARY_BASE_PORT);
        }
        
    }
    return 1;
}

int ata_write_sectors(u32 lba,u32 count, const char* data){
    if (lba > 0x0FFFFFFF)
    {
        log_error(ATA_LOG, "LBA out of range for 28-bit PIO");
        return 0;
    }

    // check the sector_number for the max in 28 bit PIO
    if (lba > 0x0FFFFFFF)
    {
        return 0;
    }

    if (count == 0)
        return 0;

    // select the drive for operation, this is the 6th register that does so, read in the theory.
    while (port_byte_in(0x1F7) & ATA_SR_BSY)
        ;
    port_byte_out(0x1F6, 0xE0 | (lba >> 24 & 0x0000000F));
    // 400 ns wait
    port_byte_in(0x1F7);
    port_byte_in(0x1F7);
    port_byte_in(0x1F7);
    port_byte_in(0x1F7);

    // now this sequence is common for both drives
    // sector count
    port_byte_out(0x1F2, count);

    // send the LBA in 3 different ports
    port_byte_out(0x1F3, (lba & 0x000000FF));       // low 8 bits
    port_byte_out(0x1F4, (lba >> 8) & 0x000000FF);  // mid 8 bits
    port_byte_out(0x1F5, (lba >> 16) & 0x000000FF); // higher 8 bits
    // this completed the 28 bit LBA

    // read command issue
    port_byte_out(0x1F7, ATA_WRITE);

    // we use polling for now for collecting data.
    u8 status_primary;
    int current_sector = 0;

    for (int sector = 0; sector < count; sector++)
    {
        while (1)
        {
            status_primary =
                port_byte_in(PRIMARY_BASE_PORT +
                             drive_offsets.status_register);

            if (status_primary & ATA_SR_ERR)
            {
                log_error(ATA_LOG, "Error writing sector to disk");
                return 0;
            }

            if (!(status_primary & ATA_SR_BSY) &&
                (status_primary & ATA_SR_DRQ))
            {
                break;
            }
        }

        // drive is ready with the data, you can read now the identify data , 512 bytes prepared by the drive, which can be read from the port in 512/2 port_out operations
        for (int i = 0; i < 256; i++)
        {
            port_word_out(PRIMARY_BASE_PORT, ((u16 *)data)[sector * 256 + i]);
        }
    }

    // flush the cache as given in the spec 
    port_byte_out(PRIMARY_BASE_PORT + drive_offsets.command_register, 0xE7);
    return 1;
}

void init_ata(){
    log_info(ATA_LOG, "Initialising ATA driver");
    // first check both the drives on the bus 
    u8 primary_drive_status = identify_drive(PRIMARY_BASE_PORT); 
    u8 secondary_drive_status = identify_drive(SECONDARY_BASE_PORT); 

    if (!primary_drive_status) log_warn(ATA_LOG, "Primary drive failed to identify");
    if (!secondary_drive_status) log_warn(ATA_LOG, "Secondary drive failed to identify");

    // now you have both drives ready to read the data. 
    register_block(&ata_driver);
    log_info(ATA_LOG, "ATA block device registered");
}