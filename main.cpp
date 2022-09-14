#include "instructions.h"
#include <iostream>

using namespace std; 

int main(int argc, char* argv[])
{

State state; 
State *ptr_state = &state ; 


printf("%d\n", ptr_state->H);

ptr_state->H = (uint8_t) 1;

uint8_t vals = (++ptr_state->H); 

uint16_t value = ptr_state->update_reg(&ptr_state->H , &ptr_state->L); 

printf("%d\n", ptr_state->H); 

// char a[5]= "stri" ;
// char *prt = a ; 

// printf("%s", *(prt+1));

uint16_t test = 0xFFAA;
uint16_t test2 = test & 0xFF;


state.H = 0x01;
state.L = 0x02;
uint16_t value_return;
value_return = state.update_reg(&state.H, &state.L);
printf("%x\n", value_return );

return 0; 
};   
