#pragma once 

// define fixed byte types 
typedef unsigned int u32; 
typedef          int s32; 
typedef unsigned short u16; 
typedef          short s16; 
typedef unsigned char u8; 
typedef          char s8;

#define low_16(address) (u16) (address & 0x0000ffff)
#define high_16(address) (u16) ((address >> 16) & 0x0000ffff)
