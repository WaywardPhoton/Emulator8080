#include "instructions.h"


// ---------------------------------------------
// ARITHMETIC GROUP
// ---------------------------------------------
void ADD_A (State* state, unsigned char *instruction, uint8_t *dest, AddressingMode mode, bool carrybool){
                                        
    // ADD to A in one of three modes, option for carry
    // encompasses ADD, ADC, ADI 

    uint16_t answer;
    switch (mode)
    {
    case REG:  answer = (uint16_t) state->A + (uint16_t) *dest + (carrybool* state->cc.cy); 
        break;
    
    case IMM:  answer = (uint16_t) state->A + (uint16_t) instruction[1]+ (carrybool* state->cc.cy);
        state->pc+=1;
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

    uint16_t HL = state->read_reg(&state-> H, &state-> L); //convert to 32 bits so that 16 bit register addition saves the carry
    uint16_t reg16 = state->read_reg(regA, regB);
    uint32_t answer = (uint32_t) HL + (uint32_t) reg16;

    state->cc.set_word_cy(answer);
}


// ---------------------------------------------
// ARITHMETIC GROUP
// ---------------------------------------------
void JNZ (State* state, unsigned char *instruction){
    if ((state->cc.z) == 0) {
        state->pc = (instruction[2] << 8) | instruction[1];
        }

    else {
        state->pc += 2; // jump not taken, increment by 2 bits beacause instruction is 3 bytes long

    }

}

void JZ (State* state, unsigned char *instruction){
    if ((state->cc.z) != 0) {
        state->pc = (instruction[2] << 8) | instruction[1];
        }

    else {
        state->pc += 2; // jump not taken, increment by 2 bits beacause instruction is 3 bytes long

    }

}


void JMP (State* state, unsigned char *instruction){

    state->pc = (instruction[2] << 8) | instruction[1];

}


void JPO (State* state, unsigned char *instruction){
    if ((state->cc.p) == 0) {
        state->pc = (instruction[2] << 8) | instruction[1];
        }

    else {
        state->pc += 2; // jump not taken, increment by 2 bits beacause instruction is 3 bytes long

    }

}


void JPE (State* state, unsigned char *instruction){
    if ((state->cc.p) == 1) {
        state->pc = (instruction[2] << 8) | instruction[1];
        }

    else {
        state->pc += 2; // jump not taken, increment by 2 bits beacause instruction is 3 bytes long

    }

}

void JNC (State* state, unsigned char *instruction){
    if ((state->cc.cy) == 0) {
        state->pc = (instruction[2] << 8) | instruction[1];
        }

    else {
        state->pc += 2; // jump not taken, increment by 2 bits beacause instruction is 3 bytes long


    }

}


void JC (State* state, unsigned char *instruction){
    if ((state->cc.cy) == 1) {
        state->pc = (instruction[2] << 8) | instruction[1];
        }

    else {
        state->pc += 2; // jump not taken, increment by 2 bits beacause instruction is 3 bytes long


    }

}

void JP (State* state, unsigned char *instruction){
    if ((state->cc.s) == 0) {
        state->pc = (instruction[2] << 8) | instruction[1];
        }

    else {
        state->pc += 2; // jump not taken, increment by 2 bits beacause instruction is 3 bytes long


    }

}


void JM (State* state, unsigned char *instruction){
    if ((state->cc.s) == 1) {
        state->pc = (instruction[2] << 8) | instruction[1];
        }

    else {
        state->pc += 2; // jump not taken, increment by 2 bits beacause instruction is 3 bytes long

    }

}

// CALL AND RET
void CALL (State* state, unsigned char *instruction){

    uint16_t ret = state->pc +2 ;  // save return address by looking at next instruction pointed to by program counter (CALL is 3 bytes long)
    state->memory[state->sp-1]= (ret >> 8) & 0xFF;              // save upper part of address on stack -1
    state->memory[state->sp-2]= ret & 0xFF;                    // save lower part of address on stack -2
    state->sp -= 2;                                           //  decrement stack pointer to point to new bottom of stack (PUSH)
    state->pc = (instruction[2] << 8) | instruction[1];      // put 16 bit address into PC
}

void RET (State* state, unsigned char *instruction){

    uint16_t ret= state->memory[state->sp]  | (state->memory[state->sp+1]) << 8 ;  // pull address from 8 bit memory locations based on SP and SP+1
    state->pc = ret;
    state-> sp +=2;                                                               // increment stack pointer to point to new bottom of stack (POP)
}

// ZERO FLAG
void CZ (State* state, unsigned char *instruction){
    if ((state->cc.z) != 0) {
    uint16_t ret = state->pc +2 ;  // save return address by looking at next instruction pointed to by program counter (CALL is 3 bytes long)
    state->memory[state->sp-1]= (ret >> 8) & 0xFF;              // save upper part of address on stack -1
    state->memory[state->sp-2]= ret & 0xFF;                    // save lower part of address on stack -2
    state->sp -= 2;                                           //  decrement stack pointer to point to new bottom of stack (PUSH)
    state->pc = (instruction[2] << 8) | instruction[1];      // put 16 bit address into PC
    }

    else {
        state->pc += 2;

    }
}

void RZ (State* state, unsigned char *instruction){
    if ((state->cc.z) != 0) {
    uint16_t ret= state->memory[state->sp]  | (state->memory[state->sp+1]) << 8 ;  // pull address from 8 bit memory locations based on SP and SP+1
    state->pc = ret;
    state-> sp +=2;  
    }                                                             // increment stack pointer to point to new bottom of stack (POP)
}

void CNZ (State* state, unsigned char *instruction){
    if ((state->cc.z) = 0) {
    uint16_t ret = state->pc +2 ;  // save return address by looking at next instruction pointed to by program counter (CALL is 3 bytes long)
    state->memory[state->sp-1]= (ret >> 8) & 0xFF;              // save upper part of address on stack -1
    state->memory[state->sp-2]= ret & 0xFF;                    // save lower part of address on stack -2
    state->sp -= 2;                                           //  decrement stack pointer to point to new bottom of stack (PUSH)
    state->pc = (instruction[2] << 8) | instruction[1];      // put 16 bit address into PC
    }

    else {
        state->pc += 2;

    }
}

void RNZ (State* state, unsigned char *instruction){
    if ((state->cc.z) = 0) {
    uint16_t ret= state->memory[state->sp]  | (state->memory[state->sp+1]) << 8 ;  // pull address from 8 bit memory locations based on SP and SP+1
    state->pc = ret;
    state-> sp +=2;  
    }                                                             // increment stack pointer to point to new bottom of stack (POP)
}

// CARRY FLAG
void CC (State* state, unsigned char *instruction){
    if ((state->cc.cy) == 1) {
    uint16_t ret = state->pc +2 ;  // save return address by looking at next instruction pointed to by program counter (CALL is 3 bytes long)
    state->memory[state->sp-1]= (ret >> 8) & 0xFF;              // save upper part of address on stack -1
    state->memory[state->sp-2]= ret & 0xFF;                    // save lower part of address on stack -2
    state->sp -= 2;                                           //  decrement stack pointer to point to new bottom of stack (PUSH)
    state->pc = (instruction[2] << 8) | instruction[1];      // put 16 bit address into PC
    }

    else {
        state->pc += 2;

    }
}

void RC (State* state, unsigned char *instruction){
    if ((state->cc.cy) == 1) {
    uint16_t ret= state->memory[state->sp]  | (state->memory[state->sp+1]) << 8 ;  // pull address from 8 bit memory locations based on SP and SP+1
    state->pc = ret;
    state-> sp +=2;  
    }                                                             // increment stack pointer to point to new bottom of stack (POP)
}

void CNC (State* state, unsigned char *instruction){
    if ((state->cc.cy) == 0) {
    uint16_t ret = state->pc +2 ;  // save return address by looking at next instruction pointed to by program counter (CALL is 3 bytes long)
    state->memory[state->sp-1]= (ret >> 8) & 0xFF;              // save upper part of address on stack -1
    state->memory[state->sp-2]= ret & 0xFF;                    // save lower part of address on stack -2
    state->sp -= 2;                                           //  decrement stack pointer to point to new bottom of stack (PUSH)
    state->pc = (instruction[2] << 8) | instruction[1];      // put 16 bit address into PC
    }

    else {
        state->pc += 2;

    }
}

void RNC (State* state, unsigned char *instruction){
    if ((state->cc.cy) == 0) {
    uint16_t ret= state->memory[state->sp]  | (state->memory[state->sp+1]) << 8 ;  // pull address from 8 bit memory locations based on SP and SP+1
    state->pc = ret;
    state-> sp +=2;  
    }                                                             // increment stack pointer to point to new bottom of stack (POP)
}

// PARITY FLAG
void CPE (State* state, unsigned char *instruction){
    if ((state->cc.p) == 1) {
    uint16_t ret = state->pc +2 ;  // save return address by looking at next instruction pointed to by program counter (CALL is 3 bytes long)
    state->memory[state->sp-1]= (ret >> 8) & 0xFF;              // save upper part of address on stack -1
    state->memory[state->sp-2]= ret & 0xFF;                    // save lower part of address on stack -2
    state->sp -= 2;                                           //  decrement stack pointer to point to new bottom of stack (PUSH)
    state->pc = (instruction[2] << 8) | instruction[1];      // put 16 bit address into PC
    }

    else {
        state->pc += 2;

    }
}

void RPE (State* state, unsigned char *instruction){
    if ((state->cc.p) == 1) {
    uint16_t ret= state->memory[state->sp]  | (state->memory[state->sp+1]) << 8 ;  // pull address from 8 bit memory locations based on SP and SP+1
    state->pc = ret;
    state-> sp +=2;  
    }                                                             // increment stack pointer to point to new bottom of stack (POP)
}

void CPO (State* state, unsigned char *instruction){
    if ((state->cc.p) == 0) {
    uint16_t ret = state->pc +2 ;  // save return address by looking at next instruction pointed to by program counter (CALL is 3 bytes long)
    state->memory[state->sp-1]= (ret >> 8) & 0xFF;              // save upper part of address on stack -1
    state->memory[state->sp-2]= ret & 0xFF;                    // save lower part of address on stack -2
    state->sp -= 2;                                           //  decrement stack pointer to point to new bottom of stack (PUSH)
    state->pc = (instruction[2] << 8) | instruction[1];      // put 16 bit address into PC
    }

    else {
        state->pc += 2;

    }
}

void RPO (State* state, unsigned char *instruction){
    if ((state->cc.p) == 0) {
    uint16_t ret= state->memory[state->sp]  | (state->memory[state->sp+1]) << 8 ;  // pull address from 8 bit memory locations based on SP and SP+1
    state->pc = ret;
    state-> sp +=2;  
    }                                                             // increment stack pointer to point to new bottom of stack (POP)
}

// SIGN FLAG
void CP (State* state, unsigned char *instruction){
    if ((state->cc.s) == 0) {
    uint16_t ret = state->pc +2 ;  // save return address by looking at next instruction pointed to by program counter (CALL is 3 bytes long)
    state->memory[state->sp-1]= (ret >> 8) & 0xFF;              // save upper part of address on stack -1
    state->memory[state->sp-2]= ret & 0xFF;                    // save lower part of address on stack -2
    state->sp -= 2;                                           //  decrement stack pointer to point to new bottom of stack (PUSH)
    state->pc = (instruction[2] << 8) | instruction[1];      // put 16 bit address into PC
    }

    else {
        state->pc += 2;

    }
}

void RP (State* state, unsigned char *instruction){
    if ((state->cc.s) == 0) {
    uint16_t ret= state->memory[state->sp]  | (state->memory[state->sp+1]) << 8 ;  // pull address from 8 bit memory locations based on SP and SP+1
    state->pc = ret;
    state-> sp +=2;  
    }                                                             // increment stack pointer to point to new bottom of stack (POP)
}

void CM (State* state, unsigned char *instruction){
    if ((state->cc.s) == 1) {
    uint16_t ret = state->pc +2 ;  // save return address by looking at next instruction pointed to by program counter (CALL is 3 bytes long)
    state->memory[state->sp-1]= (ret >> 8) & 0xFF;              // save upper part of address on stack -1
    state->memory[state->sp-2]= ret & 0xFF;                    // save lower part of address on stack -2
    state->sp -= 2;                                           //  decrement stack pointer to point to new bottom of stack (PUSH)
    state->pc = (instruction[2] << 8) | instruction[1];      // put 16 bit address into PC
    }

    else {
        state->pc += 2;

    }
}

void RM (State* state, unsigned char *instruction){
    if ((state->cc.s) == 1) {
    uint16_t ret= state->memory[state->sp]  | (state->memory[state->sp+1]) << 8 ;  // pull address from 8 bit memory locations based on SP and SP+1
    state->pc = ret;
    state-> sp +=2;  
    }                                                             // increment stack pointer to point to new bottom of stack (POP)
}
