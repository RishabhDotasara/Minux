

void main(){
    // simple kernel fro now, just tells us that it is loaded and booted.
    char* videomemory = (char*) 0xb8000; 
    *(videomemory) = 'X';
}