#include "instructions.h"
#include "Memory.h"
#include <iostream>

using namespace std; 

int main(int argc, char* argv[])
{

// State state; 
// State *ptr_state = &state ; 


// printf("%d\n", ptr_state->H);

// ptr_state->H = (uint8_t) 4;
// ptr_state->cc.all = (uint8_t) 1;


// uint8_t vals = (++ptr_state->H); 

// uint16_t value = ptr_state->read_reg(&ptr_state->H , &ptr_state->L); 

// printf("%d\n", ptr_state->cc.z); 

// char a[]= "stri" ;
// char *prt = a ; 
// printf("%c\n", *(prt+1));

// int a = 16;
// int *pointer;
// pointer = &a;  
// printf("%d\n", pointer);
// printf("%d\n", &a);


// uint16_t test = 0x0F;
// bool test2 = ParityCheck(test);
// printf("%x\n", test2);


// state.H = 0x01;
// state.L = 0x02;
// uint16_t value_return;
// value_return = state.read_reg(&state.H, &state.L);
// printf("%x\n", value_return );

State theState;
theState.A = 0xFF;
theState.B = 0xFA;
theState.cc.cy = 1; 

// printf("%x\n", (uint8_t) (0xFF + 0xFA +1 ));
// ADD_A(&theState, &theState.B, REG, true);
// printf("%x\n", theState.A);

Memory memory; 
// Memory::Config config;
// config.isRomWriteable = true;
// memory.configure(config);
// printf("%d\n", config.isRomWriteable);
Memory::Config theConfig = memory.get_config();
printf("%d\n", theConfig.isRamMirrored);
return 0; 
};   
