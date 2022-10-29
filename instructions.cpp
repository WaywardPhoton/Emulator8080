#include "instructions.h"


// ---------------------------------------------
// ARITHMETIC GROUP
// ---------------------------------------------
void ADD_A (State* state, uint8_t *dest, AddressingMode mode, bool carrybool){
                                    
    // ADD to A in one of three modes, option for carry
    // encompasses ADD, ADC, ADI 

    uint16_t answer;
    switch (mode)
    {
    case REG:  answer = (uint16_t) state->A + (uint16_t) *dest + (carrybool* state->cc.cy); 
        break;
    
    case IMM:  answer = (uint16_t) state->A + (uint16_t) state->memory[state->pc + 1]+ (carrybool* state->cc.cy);
        state->pc++;
        break;

    case ADDR:  answer = (uint16_t) state->A + state->memory[state->read_reg(&state-> H, &state-> L)]+ (carrybool* state->cc.cy);
        break;    
    }

    state->cc.set_zsp(answer);    // set zsp
    state->cc.set_byte_cy(answer);  // set carry
    state->A = (uint8_t) answer ;  // convert to 8 bit again
    
}

void SUB_A (State* state, uint8_t *dest, AddressingMode mode, bool carrybool){
    // SUB from  A in one of three modes, option for carry
    // encompasses SUB, SBB, SBI 

    uint16_t answer;
    switch (mode)
    {
    case REG:  answer = (uint16_t) state->A - (uint16_t) *dest - (carrybool* state->cc.cy); 
        break;
    
    case IMM:  answer = (uint16_t) state->A - (uint16_t) state->memory[state->pc + 1] - (carrybool* state->cc.cy);
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
    state->H = state->write_reg_A((uint16_t*) &answer);
    state->L = state->write_reg_B((uint16_t*) &answer);
    state->cc.set_word_cy(answer);
}

void DAD_SP(State* state){

    uint16_t HL = state->read_reg(&state-> H, &state-> L); //convert to 32 bits so that 16 bit register addition saves the carry
    uint32_t answer = (uint32_t) HL + (uint32_t) state->sp;
    state->H = state->write_reg_A((uint16_t*) &answer);
    state->L = state->write_reg_B((uint16_t*) &answer);
    state->cc.set_word_cy(answer);

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


void PCHL(State* state){

    state->pc = state->read_reg(&state-> H, &state-> L);   // jumps to address in HL register

}

// INTERRUPT INSTRUCTIONS

void RST(State* state, int8_t index){
    uint16_t ret = state->pc +2 ;  // save return address by looking at next instruction pointed to by program counter (CALL is 3 bytes long)
    state->memory[state->sp-1]= (ret >> 8) & 0xFF;              // save upper part of address on stack -1
    state->memory[state->sp-2]= ret & 0xFF;                    // save lower part of address on stack -2
    state->sp -= 2;                                           //  decrement stack pointer 
    state->pc = 8 * index ;                                  // jump to interrupt subroutine 


}


// LOGICAL INSTRUCTIONS

void AND_A(State* state, unsigned char *instruction, uint8_t *dest, AddressingMode mode){
                                    
    // AND with to A in one of three modes, carry is zeroes in all cases.
    // encompasses ANA, ANI and ANA M (address) 

    uint8_t answer;
    switch (mode)
    {
    case REG:  answer = state->A &  *dest ;
        break;
    
    case IMM:  answer = state->A & instruction[1];
        state->pc++;
        break;

    case ADDR:  answer = state->A & state->memory[state->read_reg(&state-> H, &state-> L)];
        break;    
    }

    state->cc.set_zsp(answer);    // set zsp
    state->cc.cy= 0;  // set carry
    state->A = answer ;  // convert to 8 bit again   
}

void ORR_A(State* state, unsigned char *instruction, uint8_t *dest, AddressingMode mode){
                                    
    // OR with to A in one of three modes, carry is zeroes in all cases.
    // encompasses ORA, ORI and ORA M (address) 

    uint8_t answer;
    switch (mode)
    {
    case REG:  answer = state->A |  *dest ;
        break;
    
    case IMM:  answer = state->A | instruction[1];
        state->pc++;
        break;

    case ADDR:  answer = state->A | state->memory[state->read_reg(&state-> H, &state-> L)];
        break;    
    }

    state->cc.set_zsp(answer);    // set zsp
    state->cc.cy= 0;  // set carry
    state->A = answer ;  // convert to 8 bit again  
}

void XOR_A(State* state, unsigned char *instruction, uint8_t *dest, AddressingMode mode){
                                    
    // XOR with to A in one of three modes, carry is zeroes in all cases.
    // encompasses XRA, XRI and XRA M (address) 

    uint8_t answer;
    switch (mode)
    {
    case REG:  answer = state->A ^  *dest ;
        break;
    
    case IMM:  answer = state->A ^ instruction[1];
        state->pc++;
        break;

    case ADDR:  answer = state->A ^ state->memory[state->read_reg(&state-> H, &state-> L)];
        break;    
    }

    state->cc.set_zsp(answer);    // set zsp
    state->cc.cy= 0;  // set carry
    state->A = answer ;  // convert to 8 bit again   
}

void CMA (State* state){

    state->A = ~state->A; 
}

// ROTATION INSTRUCTIONS
void RRC(State* state){  //Rotate accumulator right
	uint8_t bit0 = state->A & 0x01;
	state->A >>= 1;
	state->A |= (bit0 << 7);
	state->cc.cy = bit0;

}


void RLC(State* state){ //rotate accumulator left
	uint8_t bit8 = state->A & 0x80;
	state->A <<= 1;
	state->A |= (bit8);
	state->cc.cy = bit8;

}

void RAR(State* state){  //Rotate accumulator right through carry
	uint8_t bit0 = state->A & 0x01;
	state->A >>= 1;
	state->A |= (state->cc.cy << 7);
	state->cc.cy = bit0;

}


void RAL(State* state){ //rotate accumulator left through carry
	uint8_t bit8 = state->A & 0x80;
	state->A <<= 1;
	state->A |= (state->cc.cy);
	state->cc.cy = bit8;


}

// COMPARE OPERATIONS
//covers CMP, CPI

void CMP_A(State* state, unsigned char *instruction, uint8_t *dest, AddressingMode mode){
                                    
    // XOR with to A in one of three modes, carry is zeroes in all cases.
    // encompasses XRA, XRI and XRA M (address) 

    uint16_t answer;
    switch (mode)
    {
    case REG:  answer = state->A -  *dest ;
        break;
    
    case IMM:  answer = state->A - instruction[1]; //state->A -state->memory[state->pc +1];
        state->pc++;
        break;

    case ADDR:  answer = state->A - state->memory[state->read_reg(&state-> H, &state-> L)];
        break;    
    }

    state->cc.set_zsp(answer);    // set zsp
    state->cc.set_byte_cy(answer);  // set carry

}

// Set and clear CY
void CMC(State* state){

    state->cc.cy = !state->cc.cy;
}

void STC(State* state){
    state->cc.cy = 1;
}

// enable and disable interrupts
void EI (State* state){

    state->InterruptEnabled = true;   
}

void DI (State* state){

    state->InterruptEnabled = false;   
}

// PUSH and POP : Stack 

void PUSH (State* state, uint8_t *register_choice_1, uint8_t *register_choice_2){


    state->memory[state->sp-1]= *register_choice_1;              // save upper part of register on stack -1
    state->memory[state->sp-2]= *register_choice_2;              // save lower part of register on state -2
    state->sp -= 2; 

}

void POP(State* state, uint8_t *register_choice_1, uint8_t *register_choice_2){

        *register_choice_1 = state->memory[state->sp];    // pop lower part of register
        *register_choice_2 = state->memory[state->sp+1];  // pop upper part of register
        state->sp += 2;
}


void PUSH_PSW(State* state){

    state->memory[state->sp-2] = *reinterpret_cast<uint8_t*>(&state->cc);
	state->memory[state->sp-1] = state->A;
	state->sp -= 2;
   
}
void POP_PSW(State* state){

    uint8_t psw = state->memory[state->sp+1];
	state->cc = *reinterpret_cast<ConditionCodes*>(&psw);
	state->A = state->memory[state->sp + 1];
    state->sp += 2;
   
}

void SPHL(State* state){

    state->sp = state->read_reg(&state-> H, &state-> L);
    
}

void XHTL(State* state){


    uint8_t l = state->L;
	uint8_t h = state->H;
	state->L = state->memory[state->sp];
	state->H = state->memory[state->sp+1];
	state->memory[state->sp] = l;
    state->memory[state->sp+1] = h;

}

void MOV(State* state, uint8_t *register_choice_1, uint8_t *operand_2, AddressingMode mode){

    switch (mode)
    {
    case REG:  *register_choice_1 = *operand_2 ;  // Move Reg 2 into Reg 1
        break;
    
    case IMM:  *register_choice_1 = state->memory[state->pc + 1];

        state->pc++;

    case ADDR:  uint16_t address = state->read_reg(&state-> H, &state-> L);
			    state->memory[address]= *register_choice_1;  // state-> C, for example. 
        break;
    }

}


void STAX(State* state,uint8_t *register_choice_1, uint8_t *register_choice_2){

    uint16_t address = state->read_reg(register_choice_1,register_choice_2);
    state->memory[address]= state->A;

}

void LDAX(State* state,uint8_t *register_choice_1, uint8_t *register_choice_2){
 
    uint16_t address = state->read_reg(register_choice_1,register_choice_2);
    state->A = state->memory[address]; 

}

void STA(State* state, unsigned char *instruction){

    uint16_t addr = (instruction[2] << 8) | instruction[1];
    state->memory[addr]= state->A;
    state->pc += 2;

}

void LDA(State* state, unsigned char *instruction){
    uint16_t addr = (instruction[2] << 8) | instruction[1];
    state->A = state->memory[addr];
    state->pc += 2;
}

// LXI

void LXI (State* state, uint8_t *register_choice_1, uint8_t *register_choice_2,RegisterChoice choice){ //increments each register separately

    switch (choice)
    {
        case BYTE:

            *register_choice_1 = state->memory[state->pc + 2];
            *register_choice_2= state->memory[state->pc + 1];

            break;

        case WORD:  //LXI SP
            uint8_t hi = state->memory[state->pc + 2];
            uint8_t low = state->memory[state->pc + 1];
            uint16_t addr = (hi << 8) | low;
            state->sp = addr; // increment stack pointer is only option in WORD case
            break;

    state->pc+=2;

    }
}

void SHLD(State* state){
    uint8_t hi = state->memory[state->pc + 2];
    uint8_t low = state->memory[state->pc + 1];
    uint16_t addr = (hi << 8) | low;
    state->memory[addr]= state->read_reg(&state-> H, &state-> L);
    state->pc += 2;
}


void LHLD(State* state){
    uint8_t hi = state->memory[state->pc + 2];
    uint8_t low = state->memory[state->pc + 1];
    state->H= state->memory[hi];
    state->L= state->memory[low];
    state->pc += 2;


}


