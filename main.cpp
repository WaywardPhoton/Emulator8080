#include "instructions.h"
#include <iostream>

using namespace std; 

int main(int argc, char* argv[])
{

State state; 
State *ptr_state = &state ; 


printf("%d\n", ptr_state->H);

ptr_state->H = (uint8_t) 4;

uint8_t vals = (++ptr_state->H); 

uint16_t value = ptr_state->read_reg(&ptr_state->H , &ptr_state->L); 

printf("%d\n", ptr_state->H); 

// char a[]= "stri" ;
// char *prt = a ; 
// printf("%c\n", *(prt+1));

int a = 16;
int *pointer;
pointer = &a;  
printf("%d\n", pointer);
printf("%d\n", &a);


uint16_t test = 0xFFAA;
uint16_t test2 = test & 0xFF;


state.H = 0x01;
state.L = 0x02;
uint16_t value_return;
value_return = state.read_reg(&state.H, &state.L);
printf("%x\n", value_return );

return 0; 
};   
