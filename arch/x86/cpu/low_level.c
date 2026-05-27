// why this file?
// well drivers are two types, one is mempry mapped, which we can dorectly control via  by writing to the memory address. 
// second is Port I/O, where in we read and write instructions to be given to the device. for which we need to write inline assmebly, for which we will make some wrapper functions. 

// more in the book , read it , interesting 

unsigned char port_byte_in(unsigned short port){
    unsigned char result; 
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_byte_out(unsigned short port, unsigned char data){
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

unsigned short port_word_in(unsigned short port){
    unsigned short result; 
    __asm__("in %%dx, %%ax": "=a" (result) : "d" (port));
    return result; 
}

void port_word_out(unsigned short port, unsigned short data) {
    __asm__("out %%ax, %%dx": : "a" (data), "d" (port));
}
