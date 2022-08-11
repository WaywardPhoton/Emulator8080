#include "State.h"
#include <iostream>

using namespace std; 

int main(int argc, char* argv[])
{

State state; 
printf("%d\n", state.HL);

// char a[5]= "stri" ;
// char *prt = a ; 

// printf("%s", *(prt+1));
uint16_t test = 0xFFAA;
uint16_t test2 = test & 0xFF;
printf("%x\n", test2);

return 0; 
};   
