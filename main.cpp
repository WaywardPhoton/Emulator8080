
#include "CPU.h"  
#include <algorithm>
#include <array>
#include <iostream>
#include <string_view>
#include <typeinfo>
#include<functional>
#include <stdio.h>

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

// State theState;
// theState.A = 0xFF;
// theState.B = 0xFA;
// theState.cc.cy = 1; 

// printf("%x\n", (uint8_t) (0xFF + 0xFA +1 ));
// ADD_A(&theState, &theState.B, REG, true);
// printf("%x\n", theState.A);
const char* kRomFilename = "./TST8080.COM";
const uint16_t kRomLoadAddress = 0x100;

CPU emulator;
Memory memory; 

Memory::Config config;
config.sizeRam = 2000;
config.sizeRom = kRomLoadAddress + uint16_t(getFileSize(kRomFilename));
config.isRamMirrored = true;
memory.configure(config);

memory.load(kRomFilename, kRomLoadAddress);
uint16_t pc = kRomLoadAddress;

memory.write(0x05, 0xC9);

emulator.init(&memory, pc);

FILE *fp= fopen( "emulator_out.txt" , "wb" );

int i =0;
while (i <600){
    emulator.step(fp);
    i ++;

}

fclose(fp);

// Memory::Config theConfig = memory.get_config();
// printf("%d\n", theConfig.isRamMirrored);
// State s = emulator.get_state(); 
// uint8_t res = emulator.read_memory(state.B + state.C);
// printf("%x\n", res);

return 0; 
};   




// typedef std::function<uint8_t(uint8_t port)> CallbackIn;
// void setCallbackIn(CallbackIn callback);

// Callbackin first_ptr = &function;
// setCallbackIn(&function)

