// #include "io.h"
// #include "mem.h"
// #include "../kernel/kernel.h"

// char* readline(){
//     char* res[1024]; // only 1 MB input is available at once 
//     int i = 0; 
//     while (*(inp_buffer+i) != '\0'){
//         i++;
//     }

//     memory_copy(inp_buffer, res, sizeof(char)*i);
//     return res;
// }