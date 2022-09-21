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

    case ADDR:  answer = (uint16_t) state->A + state->memory[state->read_reg(&state-> H, &state-> L)]+ (carrybool* state->cc.cy);
        break;    
    }

    state->cc.set_zsp(answer);    // set zsp
    state->cc.set_byte_cy(answer);  // set carry
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

    case ADDR:  answer = (uint16_t) state->A - state->memory[state->read_reg(&state-> H, &state-> L)]- (carrybool* state->cc.cy);
        break;    
    }

    state->cc.set_zsp(answer);    // set flags
    state->cc.set_byte_cy(answer);    // set flags
    state->A = (uint8_t) answer ;  // convert to 8 bit again
    
}


void INX (State* state, uint8_t *register_choice_1, uint8_t *register_choice_2, RegisterChoice choice){ //increments each register separately
    uint16_t intermediate;
    switch (choice)
    {
        case BYTE:
            intermediate= state->read_reg(register_choice_1, register_choice_2); //&state-> L
            intermediate ++;
            *register_choice_1 = state->write_reg_A(&intermediate);
            *register_choice_2= state->write_reg_B(&intermediate);
            // *register_choice_2++;
		    // if (*register_choice_2 == 0){
			//     *register_choice_1++;
            // }
            break;

        case WORD:
            state->sp++; // increment stack pointer is only option in WORD case
            break;


    }

}


void DCX (State* state, uint8_t *register_choice_1, uint8_t *register_choice_2, RegisterChoice choice){ //increments each register separately
    uint16_t intermediate;
    switch (choice)
    {
        case BYTE:
            intermediate= state->read_reg(register_choice_1, register_choice_2);
            intermediate --;
            *register_choice_1 = state->write_reg_A(&intermediate);
            *register_choice_2= state->write_reg_B(&intermediate);
        // if (*register_choice_2 == 0){
		// 	    *register_choice_2= 0xFF;
        //         *register_choice_1--;
        //     }
            break;

        case WORD:
            state->sp--; // increment stack pointer is only option in WORD case
            break;


    }

}


void INR (State* state, uint8_t *register_choice, RegisterChoice choice){
    uint16_t answer; 
    switch (choice)
    {
    case BYTE:
        answer= ++*register_choice; // in case of BYTE, increments a register
        break;
    
    case WORD:
        uint16_t offset = state->read_reg(&state-> H, &state-> L); // in the case of WORD, it increments the value stored in memory at address (HL)
        answer= ++(state-> memory[offset]);
        break;
    }

    state->cc.set_zsp(answer);  // set only zsp and not carry bit

}

void DCR (State* state, uint8_t *register_choice, RegisterChoice choice){
    uint16_t answer; 
    switch (choice)
    {
    case BYTE:
        answer= --*register_choice; // in case of BYTE, increments a register
        break;
    
    case WORD:
        uint16_t offset = state->read_reg(&state-> H, &state-> L); // in the case of WORD, it increments the value stored in memory at address (HL)
        answer= --(state-> memory[offset]);
        break;
    }

    state->cc.set_zsp(answer);  // set only zsp and not carry bit

}

void DAD(State* state, uint8_t *regA, uint8_t *regB ){

    uint16_t HL = state->read_reg(&state-> H, &state-> L);
    uint16_t reg16 = state->read_reg(regA, regB);
    uint32_t answer = (uint32_t) HL + (uint32_t) reg16;
    state->cc.set_word_cy(answer);
}


