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

    case ADDR:  answer = (uint16_t) state->A + state->memory[state->update_reg(&state-> H, &state-> L)]+ (carrybool* state->cc.cy);
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

    case ADDR:  answer = (uint16_t) state->A - state->memory[state->update_reg(&state-> H, &state-> L)]- (carrybool* state->cc.cy);
        break;    
    }

    state->cc.set_zspcy(answer);    // set flags
    state->A = (uint8_t) answer ;  // convert to 8 bit again
    
}


void INX (State* state, RegisterChoice choice){ // index is 0 for BC, 1 for DE, 2 for HL, 3 for SP
switch (choice){
    case BC: 
        state->C++;
		if (state->C == 0)
			state->B++;
        break;
    case DE: 
        state->E++;
		if (state->E == 0)
			state->D++;
        break;
    case HL:
        state->L++;
		if (state->L == 0)
			state->H++;
        break;
    case SP:     
        state->sp ++ ; 

}

}

void INR (State* state, RegisterChoice choice){ // index is 0 for BC, 1 for DE, 2 for HL, 3 for SP figure out INR M 
switch (choice){
    uint8_t answer; 
    case A: 
        answer= ++state->A;
        break;
    case B: 
        answer = ++state->B;
        break;
    case C:
        answer = ++state->C;
        break;
    case D:
        answer = ++state->D;
        break;
    case E:
        answer = ++state->E;
        break;
    case H:     
        answer= ++state->H; 
        break;
    case L:     
        answer= ++state->L; 
        break;


    state->cc.set_zspcy((uint16_t)answer); 

}

}


