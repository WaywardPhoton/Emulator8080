#include "instructions.h"

void ADD_A (State* state, unsigned char *instruction, uint8_t *dest, AddressingMode mode, bool carrybool){
    // ADD to A in one of three modes, option for carry
    // encompasses ADD, ADC, ADI 

    uint16_t answer;
    switch (mode)
    {
    case REG:  answer = (uint16_t) state->A + (uint16_t) *dest + (carrybool* state->cc.cy); 
        break;
    
    case IMM:  answer = (uint16_t) state->A + (uint16_t) instruction[1]+ (carrybool* state->cc.cy);
        break;

    case ADDR:  answer = (uint16_t) state->A + state->memory[state->HL.get()]+ (carrybool* state->cc.cy);
        break;    
    }

    state->cc.set_zspcy(answer);    // set flags
    state->A = (uint8_t) answer ;  // convert to 8 bit again
    
}

void SUB_A (State* state, unsigned char *instruction, uint8_t *dest, AddressingMode mode, bool carrybool){
    // SUB from  A in one of three modes, option for carry
    // encompasses SUB, SBB, SBI 

    uint16_t answer;
    switch (mode)
    {
    case REG:  answer = (uint16_t) state->A - (uint16_t) *dest - (carrybool* state->cc.cy); 
        break;
    
    case IMM:  answer = (uint16_t) state->A - (uint16_t) instruction[1]- (carrybool* state->cc.cy);
        break;

    case ADDR:  answer = (uint16_t) state->A - state->memory[state->HL.get()]- (carrybool* state->cc.cy);
        break;    
    }

    state->cc.set_zspcy(answer);    // set flags
    state->A = (uint8_t) answer ;  // convert to 8 bit again
    
}