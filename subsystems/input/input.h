
// to be used by drivers 
void input_push(char chr); // for the devices to push to the buffer, for now for keyboard. 
void input_pop();
char* get_inp_buffer();
int input_available(); 

//  public function to be used by other subsystems 
void init_input(); //init input subsystem 
char input_getc();