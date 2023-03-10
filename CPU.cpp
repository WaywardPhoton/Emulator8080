#include "CPU.h"
#include "helpers.h"


CPU::CPU(): memory(nullptr){
    reset_num_steps();
    state.reset();
}


void CPU::init(Memory* input_memory, uint16_t pc_start){
    memory = input_memory;
    state.pc = pc_start;
    reset_num_steps();



}

void CPU::reset_num_steps(){
    num_steps = 0;
}

int& CPU::get_num_steps(){
    return num_steps;
}

State CPU::get_state(){
    return state;
}

    // generate interrupt
void CPU::interrupt (int interrupt_index){

    uint16_t ret = state.pc +2 ;  // save return address by looking at next instruction pointed to by program counter (CALL is 3 bytes long)
    write_memory(state.sp-1, ((ret >> 8) & 0xFF));               // save upper part of address on stack -1         
    write_memory(state.sp-2,(ret & 0xFF));                     // save lower part of address on stack -2
    state.sp -= 2;                                             //  decrement stack pointer 
    state.pc = 8 * interrupt_index ;                                   // jump to interrupt subroutine 

}

uint16_t CPU::read_opcode_word() const{

    uint16_t opcode = create_word(read_memory(state.pc+2), read_memory(state.pc+1));
    return opcode;
}

uint8_t CPU::read_memory(uint16_t address) const {

    uint16_t addr = memory->translate(address);
    uint8_t value = memory->read(addr);
    return value;
}

void CPU::write_memory(uint16_t address ,uint8_t value){

    uint16_t addr = memory -> translate(address);
    memory->write(addr, value);
}

void CPU::call(uint16_t jmp_addr, uint16_t ret_addr ){
    write_memory(state.sp-1, ((ret_addr >> 8) & 0xFF));     // save upper part of address on stack -1
    write_memory(state.sp-2, (ret_addr & 0xFF));            // save lower part of address on stack -2
    state.sp -= 2;                                           //  decrement stack pointer to point to new bottom of stack (PUSH)
    state.pc = jmp_addr;      
    
                                  // put 16 bit address into PC
}

void CPU::ret (){
    uint16_t ret_addr= create_word(read_memory(state.sp+1), read_memory(state.sp));
    state.pc = ret_addr;                          // pull address from 8 bit memory locations based on SP and SP+1
    state.sp +=2;                                                               // increment stack pointer to point to new bottom of stack (POP)
}

void CPU::MOV(uint8_t *register_choice_1, uint8_t *operand_2, AddressingMode mode){
    switch (mode)
    {
    case REG: 
        {
        *register_choice_1 = *operand_2 ;  // Move Reg 2 into Reg 1
        break;
        }
    
    case IMM:   
        {
        uint16_t address = state.read_reg(&state.H, &state.L);
        *register_choice_1 = read_memory(address);  // state-> C, for example. 
        break;
        }
    case ADDR:
        {
        uint16_t address = state.read_reg(&state.H, &state.L);
        write_memory(address, *register_choice_1);
        break;
        }
    }
}
void CPU::ADD(uint8_t *dest, AddressingMode mode, bool carrybool){
    uint16_t answer;
    switch (mode)
    {
    case REG: 
        {

            answer = (uint16_t) state.A + (uint16_t) *dest + (carrybool* state.cc.cy); 
            break;
        }
    
    case ADDR:
        {
            uint16_t addr = state.read_reg(&state.H, &state.L);
            answer = (uint16_t) state.A + read_memory(addr)+ (carrybool* state.cc.cy);
            break;   
        } 
    }

    state.cc.set_zsp(answer);    // set zsp
    state.cc.set_byte_cy(answer);  // set carry
    state.A = (uint8_t) answer ;  // convert to 8 bit again
    
}
void CPU::SUB(uint8_t *dest, AddressingMode mode, bool carrybool){
    uint16_t answer;
    switch (mode)
    {
    case REG: 
        {

            answer = (uint16_t) state.A - (uint16_t) *dest - (carrybool* state.cc.cy); 
            break;
        }
    
    case ADDR:
        {
            uint16_t addr = state.read_reg(&state.H, &state.L);
            answer = (uint16_t) state.A -read_memory(addr)- (carrybool* state.cc.cy);
            break;   
        } 
    }

    state.cc.set_zsp(answer);    // set zsp
    state.cc.set_byte_cy(answer);  // set carry
    state.A = (uint8_t) answer ;  // convert to 8 bit again
    
}
void CPU::AND(uint8_t *dest, AddressingMode mode){                               
    // AND with to A in one of three modes, carry is zeroes in all cases.
    // encompasses ANA, ANI and ANA M (address) 

    uint8_t answer;
    switch (mode)
    {
    case REG:  {answer = state.A &  *dest ;
        break;
    }
    

    case ADDR:  {
        uint16_t addr = state.read_reg(&state.H, &state.L);
        answer = state.A & read_memory(addr);
        break;  
    }  
    }

    state.cc.set_zsp(answer);    // set zsp
    state.cc.cy= 0;  // set carry
    state.A = answer ;  // convert to 8 bit again   
}
void CPU::ORR(uint8_t *dest, AddressingMode mode){
                                
    uint8_t answer;
    switch (mode)
    {
    case REG:
    {
        answer = state.A |  *dest ;
        break;
    }
    
    case ADDR:  
    {
        uint16_t addr = state.read_reg(&state.H, &state.L);
        answer = state.A | read_memory(addr);
        break; 
    }  
    } 

    state.cc.set_zsp(answer);    // set zsp
    state.cc.cy= 0;  // set carry
    state.A = answer ;  // convert to 8 bit again  
}
void CPU::XOR(uint8_t *dest, AddressingMode mode){
                                    
    // XOR with to A in one of three modes, carry is zeroes in all cases.
    // encompasses XRA and XRA M (address) 

    uint8_t answer;
    switch (mode)
    {
    case REG:  
    {
        answer = state.A ^  *dest ;
        break;
    }
    
    case ADDR:
    {
        uint16_t addr = state.read_reg(&state.H, &state.L);
        answer = state.A ^ read_memory(addr);
        
        break;   
    }

    }

    state.cc.set_zsp(answer);    // set zsp
    state.cc.cy= 0;  // set carry
    state.A = answer ;  // convert to 8 bit again   
}
void CPU::CMP(uint8_t *dest, AddressingMode mode){                          
    uint16_t answer;
    switch (mode)
    {
    case REG:
    {  answer = state.A -  *dest ;
        break;

    }
    case ADDR:
    
        uint16_t addr = state.read_reg(&state.H, &state.L);{
        answer = state.A - read_memory(addr);
        break;    
    }
    }

    state.cc.set_zsp(answer);    // set zsp
    state.cc.set_byte_cy(answer);  // set carry

}
void CPU::JMP(){
    uint16_t address = read_opcode_word();
    state.pc = address;

}

void CPU::step(FILE* fp){
    num_steps+=1;
    int opcode_size = 1; 
    uint8_t opcode = read_memory(state.pc);
    fprintf(fp, "%04x %s", opcode, " "); 
    fprintf(fp, "A: %02x  B: %02x  C: %02x  D: %02x  E: %02x  H: %02x  L: %02x  s: %01x  p: %01x  z: %01x  cy: %01x  %s", state.A,state.B,state.C,state.D,state.E,state.H,state.L,state.cc.s,state.cc.p, state.cc.z, state.cc.cy, "\n"); 


    switch(opcode)
    {
        case 0x00: break;       //NOP                     
        case 0x01:      // LXI B, D16
        {
            state.B = read_memory(state.pc+2);   
            state.C = read_memory(state.pc+1);
            opcode_size = 3;
            break;
        }

        case 0x02:      //STAX B
        {
            uint16_t address = state.read_reg(&state.B,&state.C); 
            write_memory(address, state.A);
            break;
        }


        case 0x03:      // INX B
        {
            uint16_t intermediate= state.read_reg(&state.B, &state.C); 
            intermediate ++;
            state.B = state.write_reg_A(&intermediate);
            state.C = state.write_reg_B(&intermediate);
            break;
        }

        case 0x04:      //INR B
        {
            uint8_t answer = ++state.B;    
            state.cc.set_zsp(answer);
            break;
        }

        case 0x05:      //DCR B 
        {
            uint8_t answer = --state.B;   
            state.cc.set_zsp(answer);
            break;
        }
        
        case 0x06:      //MVI B, D8
        {                             
            state.B= read_memory(state.pc+1);
            opcode_size= 2;
            break;
        }

        case 0x07:      //RLC  
        {                           
            uint8_t bit8 = state.A & 0x80;
            state.A <<= 1;
            state.A |= (bit8);
            state.cc.cy = bit8;
            break;
        }

        case 0x09:     //DAD B
        {
            uint16_t HL = state.read_reg(&state.H, &state.L); //convert to 32 bits so that 16 bit register addition saves the carry
            uint16_t BC = state.read_reg(&state.B, &state.C);
            uint32_t answer = (uint32_t) HL + (uint32_t) BC;
            state.H = state.write_reg_A((uint16_t*) &answer);
            state.L = state.write_reg_B((uint16_t*) &answer);
            state.cc.set_word_cy(answer);
            break;
        }

        case 0x0a:    //LDAX B
        {
            uint16_t address = state.read_reg(&state.B, &state.C);
            state.A = read_memory(address); 
            break;
        }

        case 0x0b:{     //DCX B
            uint16_t intermediate= state.read_reg(&state.B, &state.C);
            intermediate --;
            state.B = state.write_reg_A(&intermediate);
            state.C= state.write_reg_B(&intermediate);
            break;

        }

        case 0x0c:      //INR C
        {
            uint8_t answer = ++state.C;   
            state.cc.set_zsp(answer);
            break;
        }
        case 0x0d:      //DCR C
        {
            uint8_t answer = --state.C;   
            state.cc.set_zsp(answer);
            break;
        }

        case 0x0e:      //MVI C, D8
        {
            state.C = read_memory(state.pc+1); 
            opcode_size= 2;
            break;
        }

        case 0x0f:      //RRC
        {
            uint8_t bit0 = state.A & 0x01;
            state.A >>= 1;
            state.A |= (bit0 << 7);
            state.cc.cy = bit0;
            break;

        }

        case 0x11:      // LXI D, D16
        {
            state.D = read_memory(state.pc+2);   
            state.E = read_memory(state.pc+1);
            opcode_size = 3;
            break; 
        }

        case 0x12:      //STAX D
        {
            uint16_t address = state.read_reg(&state.D,&state.E); 
            write_memory(address, state.A);
            break;
        }

        case 0x13:      // INX D
        {
            uint16_t intermediate= state.read_reg(&state.D, &state.E); 
            intermediate ++;
            state.D = state.write_reg_A(&intermediate);
            state.E = state.write_reg_B(&intermediate);
            break;
        }

        case 0x14:      //INR D
        {
            uint8_t answer = ++state.D;   
            state.cc.set_zsp(answer);
            break;
        }

        case 0x15:      //DCR D
        {
            uint8_t answer = --state.D;   
            state.cc.set_zsp(answer);
            break;
        }
        
        case 0x16:      //MVI D, D8
        {                           
            state.D= read_memory(state.pc+1);
            opcode_size= 2;
            break;
        }
        
        case 0x17:      //RAL
        {
            uint8_t bit8 = state.A & 0x80;
            state.A <<= 1;
            state.A |= (state.cc.cy);
            state.cc.cy = bit8;
            break;
        }

        case 0x19:      //DAD D
        {
            uint16_t HL = state.read_reg(&state.H, &state.L); //convert to 32 bits so that 16 bit register addition saves the carry
            uint16_t DE = state.read_reg(&state.D, &state.E);
            uint32_t answer = (uint32_t) HL + (uint32_t) DE;
            state.H = state.write_reg_A((uint16_t*) &answer);
            state.L = state.write_reg_B((uint16_t*) &answer);
            state.cc.set_word_cy(answer);
            break;
        }

        case 0x1a:      //LDAX D
        {
            uint16_t address = state.read_reg(&state.D, &state.E);
            state.A = read_memory(address); 
            break;
        }

        case 0x1b:      //DCX D`
        {     
            uint16_t intermediate= state.read_reg(&state.D, &state.E);
            intermediate --;
            state.D = state.write_reg_A(&intermediate);
            state.E= state.write_reg_B(&intermediate);
            break;

        }
        
        case 0x1c:      //INR E 
        {
            uint8_t answer = ++state.E;   
            state.cc.set_zsp(answer);
            break;
        }

        case 0x1d:      //DCR E
        {
            uint8_t answer = --state.E;   
            state.cc.set_zsp(answer);
            break;
        }

        case 0x1e:      //MVI E, D8
        {
            state.E= read_memory(state.pc+1); 
            opcode_size= 2;
            break;
        }

        case 0x1f:      //RAR
        {
            uint8_t bit0 = state.A & 0x01;
            state.A >>= 1;
            state.A |= (state.cc.cy << 7);
            state.cc.cy = bit0;
            break;
        }

        case 0x21:      // LXI H, D16
        {
            state.H = read_memory(state.pc+2);   
            state.L = read_memory(state.pc+1);
            opcode_size = 3;
            break;  
        }

        case 0x22:      //SHLD ADR
        {
            uint16_t address = read_opcode_word();
            write_memory(address, state.L);
            write_memory(address+1, state.H);
            opcode_size = 3;
            break;
        }

        case 0x23:      // INX H
        { 
            uint16_t intermediate= state.read_reg(&state.H, &state.L); 
            intermediate ++;
            state.H = state.write_reg_A(&intermediate);
            state.L = state.write_reg_B(&intermediate);
            break;
        }

        case 0x24:      //INR H 
        {
            uint8_t answer = ++state.H;   
            state.cc.set_zsp(answer);
            break;
        } 

        case 0x25:      //DCR H
        {
            uint8_t answer = --state.H;   
            state.cc.set_zsp(answer);
            break;
        }

        case 0x26:      //MVI H, D8
        {                           
            state.H= read_memory(state.pc+1);
            opcode_size= 2;
            break;
        }

        case 0x27:      //DAA
        {
            if ((state.A & 0x0f) > 9) {
				state.A += 6;
			}

            if ((1 == state.cc.cy) || ((state.A & 0xf0) > 0x90)) {
                state.A += 0x60;
                state.cc.cy = 1;
                state.cc.set_zsp(state.A);
            }
            break;
        }


        case 0x29:     //DAD H
        {
            uint16_t HL = state.read_reg(&state.H, &state.L); //convert to 32 bits so that 16 bit register addition saves the carry
            uint32_t answer = (uint32_t) HL + (uint32_t) HL;
            state.H = state.write_reg_A((uint16_t*) &answer);
            state.L = state.write_reg_B((uint16_t*) &answer);
            state.cc.set_word_cy(answer);
            break;
        }

        case 0x2a:      //LHLD ADR
        {
            uint16_t addr = read_opcode_word();
            state.H= read_memory(addr+1);
            state.L= read_memory(addr);
            opcode_size = 3;
            break;
        }

        case 0x2b:      //DCX H
        {     
            uint16_t intermediate= state.read_reg(&state.H, &state.L);
            intermediate --;
            state.H = state.write_reg_A(&intermediate);
            state.L= state.write_reg_B(&intermediate);
            break;

        }

        case 0x2c:      //INR L
        {
            uint8_t answer = ++state.L;   
            state.cc.set_zsp(answer);
            break;
        }

        case 0x2d:      //DCR L
        {
            uint8_t answer = --state.L;   
            state.cc.set_zsp(answer);
            break;
        }

        case 0x2e:      //MVI L, D8
        {
            state.L= read_memory(state.pc+1); 
            opcode_size= 2;
            break;
        }

        case 0x2f:      //CMA
        {
            state.A = ~state.A; 
            break;
        }

        case 0x31:      // :LXI SP, D16
        {
            state.sp = read_opcode_word();    
            opcode_size = 3;
            break;

        }

        case 0x32:      //STA ADR
        {
            uint16_t addr = read_opcode_word();
            write_memory(addr, state.A);
            opcode_size = 3;
            break;
        }

        case 0x33:      //INX SP
        {
            state.sp++; 
            break;

        }

        case 0x34:      // INR M
        {
            uint16_t address = state.read_reg(&state.H, &state.L); 
            uint16_t answer= (read_memory(address)) + 1 ;
            state.cc.set_zsp(answer);
            write_memory(address, uint8_t(answer & 0xff));
            break;
        }

        case 0x35:      // DCR M
        {
            uint16_t address = state.read_reg(&state.H, &state.L); 
            uint16_t answer= (read_memory(address)) - 1;
            state.cc.set_zsp(answer);
            write_memory(address, uint8_t(answer & 0xff));
            break;
        }


        case 0x36:      //MVI M, D8
        {                           
            uint16_t address = state.read_reg(&state.H, &state.L);
            uint8_t value = read_memory(state.pc + 1);
			write_memory(address, value);
            opcode_size= 2;
            break;
        }

        case 0x37:      //STC
        {
            state.cc.cy = 1;
            break;
        }

        case 0x39:      //DAD SP
        {
            uint16_t HL = state.read_reg(&state.H, &state.L); //convert to 32 bits so that 16 bit register addition saves the carry
            uint32_t answer = (uint32_t) HL + (uint32_t) state.sp;
            state.H = state.write_reg_A((uint16_t*) &answer);
            state.L = state.write_reg_B((uint16_t*) &answer);
            state.cc.set_word_cy(answer);
            break;
        }

        case 0x3a:      //LDA ADDR
        {
            uint16_t addr = read_opcode_word();
            state.A = read_memory(addr);
            opcode_size = 3;
            break;
        }

        case 0x3b:      //DCX SP
        {
            state.sp--;
            break;
        }

        case 0x3c:      //INR A 
        {
            uint8_t answer = ++state.A;        
            state.cc.set_zsp(answer);
            break;

        }
        case 0x3d:      //DCR A
        {
            uint8_t answer = --state.A;         
            state.cc.set_zsp(answer);
            break;
        }

        case 0x3e:      //MVI A, D8
        {                                 
            state.A= read_memory(state.pc+1); 
            opcode_size= 2;
            break;
        }

        case 0x3f:      //CMC
        {
            state.cc.cy = !state.cc.cy;
            break;
        }
        // MOV

        case 0x41:      //MOV B,C
        {
            MOV(&state.B, &state.C, REG);
            break;
        }
        case 0x42:      //MOV B,D
        {
            MOV(&state.B, &state.D, REG);
            break;
        }
        case 0x43:      //MOV B,E
        {
            MOV(&state.B, &state.E, REG);
            break;
        }
        case 0x44:      //MOV B,H
        {
            MOV(&state.B, &state.H, REG);
            break;
        }
        case 0x45:      //MOV B,L
        {
            MOV(&state.B, &state.L, REG);
            break;
        }
        case 0x46:      //MOV B,(HL)
        {
            MOV(&state.B, NULL, IMM);
            break;
        }
        case 0x47:
        {
            MOV(&state.B, &state.A, REG);
            break;
        }
         case 0x48:      //MOV C,B
        {
            MOV(&state.C, &state.B, REG);
            break;
        }
        case 0x4A:      //MOV C,D
        {
            MOV(&state.C, &state.D, REG);
            break;
        }
        case 0x4B:      //MOV C,E
        {
            MOV(&state.C, &state.E, REG);
            break;
        }
        case 0x4C:      //MOV C,H
        {
            MOV(&state.C, &state.H, REG);
            break;
        }
        case 0x4D:      //MOV C,L
        {
            MOV(&state.C, &state.L, REG);
            break;
        }
        case 0x4E:      //MOV B,(HL)
        {
            MOV(&state.C, NULL, IMM);
            break;
        }
        case 0x4F:
        {
            MOV(&state.C, &state.A, REG);
            break;
        }
        case 0x50:      //MOV D,B
        {
            MOV(&state.D, &state.B, REG);
            break;
        }
        case 0x51:      //MOV D,C
        {
            MOV(&state.D, &state.C, REG);
            break;
        }
        case 0x53:      //MOV D,E
        {
            MOV(&state.D, &state.E, REG);
            break;
        }
    
        case 0x54:      //MOV D,H
        {
            MOV(&state.D, &state.H, REG);
            break;
        }
        case 0x55:      //MOV D,L
        {
            MOV(&state.D, &state.L, REG);
            break;
        }
        case 0x56:      //MOV D,(HL)
        {
            MOV(&state.D, NULL, IMM);
            break;
        }
        case 0x57:      //MOV D,A
        {
            MOV(&state.D, &state.A, REG);
            break;
        }
        case 0x58:      //MOV E,B
        {
            MOV(&state.E, &state.B, REG);
            break;
        }
        case 0x59:      //MOV E,D
        {
            MOV(&state.E, &state.C, REG);
            break;
        }
        case 0x5A:      //MOV E,E
        {
            MOV(&state.E, &state.D, REG);
            break;
        }
        case 0x5C:      //MOV E,H
        {
            MOV(&state.E, &state.H, REG);
            break;
        }
        case 0x5D:      //MOV E,L
        {
            MOV(&state.E, &state.L, REG);
            break;
        }
        case 0x5E:      //MOV E,(HL)
        {
            MOV(&state.E, NULL, IMM);
            break;
        }
        case 0x5F:      //MOV E,A
        {
            MOV(&state.E, &state.A, REG);
            break;
        }
        case 0x60:      //MOV H,B
        {
            MOV(&state.H, &state.B, REG);
            break;
        }
        case 0x61:      //MOV H,C
        {
            MOV(&state.H, &state.C, REG);
            break;
        }
        case 0x62:      //MOV H,D
        {
            MOV(&state.H, &state.D, REG);
            break;
        }
        case 0x63:      //MOV H,E
        {
            MOV(&state.H, &state.E, REG);
            break;
        }
        case 0x65:      //MOV H,L
        {
            MOV(&state.H, &state.L, REG);
            break;
        }
        case 0x66:      //MOV H,(HL)
        {
            MOV(&state.H, NULL, IMM);
            break;
        }
        case 0x67:
        {
            MOV(&state.H, &state.A, REG);
            break;
        }
        case 0x68:      //MOV L,B
        {
            MOV(&state.L, &state.B, REG);
            break;
        }
        case 0x69:      //MOV L,C
        {
            MOV(&state.L, &state.C, REG);
            break;
        }
        case 0x6A:      //MOV L,D
        {
            MOV(&state.L, &state.D, REG);
            break;
        }
        case 0x6B:      //MOV L,E
        {
            MOV(&state.L, &state.E, REG);
            break;
        }
        case 0x6C:      //MOV L,H
        {
            MOV(&state.L, &state.H, REG);
            break;
        }
        case 0x6E:      //MOV L,(HL)
        {
            MOV(&state.L, NULL, IMM);
            break;
        }
        case 0x6F: 
        {
            MOV(&state.L, &state.A, REG);
            break;
        }
        case 0x70:      // MOV (HL), B
        {
            MOV(&state.B, NULL, ADDR);
            break;
        }
        case 0x71:      // MOV (HL), C
        {
            MOV(&state.C, NULL, ADDR);
            break;
        }
        case 0x72:      // MOV (HL), D
        {
            MOV(&state.D, NULL, ADDR);
            break;
        }
        case 0x73:      // MOV (HL), E
        {
            MOV(&state.E, NULL, ADDR);
            break;
        }
        case 0x74:      // MOV (HL), H
        {
            MOV(&state.H, NULL, ADDR);
            break;
        }
        case 0x75:      // MOV (HL), L
        {
            MOV(&state.L, NULL, ADDR);
            break;
        }
        case 0x77:      // MOV (HL), A
        {
            MOV(&state.A, NULL, ADDR);
            break;
        }
        case 0x78:      //MOV A,B
        {
            MOV(&state.A, &state.B, REG);
            break;
        }
        case 0x79:      //MOV A,C
        {
            MOV(&state.A, &state.C, REG);
            break;
        }
        case 0x7A:      //MOV A,D
        {
            MOV(&state.A, &state.D, REG);
            break;
        }
        case 0x7B:      //MOV A,E
        {
            MOV(&state.A, &state.E, REG);
            break;
        }
        case 0x7C:      //MOV A,H
        {
            MOV(&state.A, &state.H, REG);
            break;
        }
        case 0x7D:      //MOV A,L
        {
            MOV(&state.A, &state.L, REG);
            break;
        }
        case 0x7E:
        {
            MOV(&state.A, NULL, IMM);
            break;
        }
// ADD 
        case 0x80:      //ADD B
        {
            ADD(&state.B, REG, false );
            break;

        }
        case 0x81:      //ADD C
        {
            ADD(&state.C, REG, false );
            break;
        }
        case 0x82:      //ADD D
        {
            ADD(&state.D, REG, false );
            break;
        }
        case 0x83:      //ADD E
        {
            ADD(&state.E, REG, false );
            break;
        }
        case 0x84:      //ADD H
        {
            ADD(&state.H, REG, false );
            break;
        }
        case 0x85:      //ADD L
        {
            ADD(&state.L, REG, false );
            break;
        }
        case 0x86:      //ADD M
        {
            ADD(NULL, ADDR, false );
            break;
        }
        case 0x87:      //ADD A
        {
            ADD(&state.A, REG, false );
            break;
        }
        case 0x88:      //ADC B
        {
            ADD(&state.B, REG, true );
            break;
        }
        case 0x89:      //ADC C
        {
            ADD(&state.C, REG, true );
            break;
        }
        case 0x8A:      //ADC D
        {
            ADD(&state.D, REG, true );
            break;
        }
        case 0x8B:      //ADC E
        {
            ADD(&state.E, REG, true );
            break;
        }
        case 0x8C:      //ADC H
        {
            ADD(&state.H, REG, true );
            break;
        }
        case 0x8D:      //ADC L
        {
            ADD(&state.L, REG, true );
            break;
        }
        case 0x8E:      //ADC M
        {
            ADD(NULL, ADDR, true );
            break;
        }
        case 0x8F:      //ADC A
        {
            ADD(&state.A, REG, true );
            break;
        }
// SUB 

        case 0x90:      //SUB B
        {
            SUB(&state.B, REG, false );
            break;
        }
        case 0x91:      //SUB C
        {
            SUB(&state.C, REG, false );
            break;
        }
        case 0x92:      //SUB D
        {
            SUB(&state.D, REG, false );
            break;
        }
        case 0x93:      //SUB E
        {
            SUB(&state.E, REG, false );
            break;
        }
        case 0x94:      //SUB H
        {
            SUB(&state.H, REG, false );
            break;
        }
        case 0x95:      //SUB L
        {
            SUB(&state.L, REG, false );
            break;
        }
        case 0x96:      //SUB M
        {
            SUB(NULL, ADDR, false );
            break;
        }
        case 0x97:      //SUB A
        {
            SUB(&state.A, REG, false );
            break;
        }
        case 0x98:      //SBB B
        {
            SUB(&state.B, REG, true );
            break;
        }
        case 0x99:      //SBB C
        {
            SUB(&state.C, REG, true );
            break;
        }
        case 0x9A:      //SBB D
        {
            SUB(&state.D, REG, true );
            break;
        }
        case 0x9B:      //SBB E
        {
            SUB(&state.E, REG, true );
            break;
        }
        case 0x9C:      //SBB H
        {
            SUB(&state.H, REG, true );
            break;
        }
        case 0x9D:      //SBB L
        {
            SUB(&state.L, REG, true );
            break;
        }
        case 0x9E:      //SBB M
        {
            SUB(NULL, ADDR, true );
            break;
        }
        case 0x9F:      //SBB A
        {
           SUB(&state.A, REG, true );
           break;
        }

// ANA

        case 0xA0:      //ANA B
        {
            AND(&state.B, REG);
            break;
        }
        case 0xA1:      //ANA C
        {
            AND(&state.C, REG);
            break;
        }
        case 0xA2:      //ANA D
        {
            AND(&state.D, REG);
            break;
        }
        case 0xA3:      //ANA E
        {
            AND(&state.E, REG);
            break;
        }
        case 0xA4:      //ANA H
        {
            AND(&state.H, REG);
            break;
        }
        case 0xA5:      //ANA L
        {
            AND(&state.L, REG);
            break;
        }
        case 0xA6:      //ANA M
        {
            AND(NULL, ADDR);
            break;
        }
        case 0xA7:      //ANA A
        {
            AND(&state.A, REG);
            break;
        }
// XOR   
        case 0xA8:      //XOR B
        {
            XOR(&state.B, REG);
            break;
        }
        case 0xA9:      //XOR C
        {
            XOR(&state.C, REG);
            break;
        }
        case 0xAA:      //XOR D
        {
            XOR(&state.D, REG);
            break;
        }
        case 0xAB:      //XOR E
        {
            XOR(&state.E, REG);
            break;
        }
        case 0xAC:      //XOR H
        {
            XOR(&state.H, REG);
            break;
        }
        case 0xAD:      //XOR L
        {
            XOR(&state.L, REG);
            break;
        }
        case 0xAE:      //XOR M
        {
            XOR(NULL, ADDR);
            break;
        }
        case 0xAF:      //XOR A
        {
            XOR(&state.A, REG);
            break;
        }
   
// ORR
        case 0xB0:      //ORR B
        {
            ORR(&state.B, REG);
            break;
        }
        case 0xB1:      //ORR C
        {
            ORR(&state.C, REG);
            break;
        }
        case 0xB2:      //ORR D
        {
            ORR(&state.D, REG);
            break;
        }
        case 0xB3:      //ORR E
        {
            ORR(&state.E, REG);
            break;
        }
        case 0xB4:      //ORR H
        {
            ORR(&state.H, REG);
            break;
        }
        case 0xB5:      //ORR L
        {
            ORR(&state.L, REG);
            break;
        }
        case 0xB6:      //ORR M
        {
            ORR(NULL, ADDR);
            break;
        }
        case 0xB7:      //ORR A
        {
            ORR(&state.A, REG);
            break;
        }
    
// CMP
        case 0xB8:      //CMP B
        {
            CMP(&state.B, REG);
            break;
        }
        case 0xB9:      //CMP C
        {
            CMP(&state.C, REG);
            break;
        }
        case 0xBA:      //CMP D
        {
            CMP(&state.D, REG);
            break;
        }
        case 0xBB:      //CMP E
        {
            CMP(&state.E, REG);
            break;
        }
        case 0xBC:      //CMP H
        {
            CMP(&state.H, REG);
            break;
        }
        case 0xBD:      //CMP L
        {
            CMP(&state.L, REG);
            break;
        }
        case 0xBE:      //CMP M
        {
           CMP(NULL, ADDR);
           break;
        }
        case 0xBF:      //CMP A
        {
            CMP(&state.A, REG);
            break;
        }
        case 0xC0:      //RNZ
        {
            if ((state.cc.z) == 0){
                ret();
                opcode_size = 0 ; 
            }
            break;
        } 

        case 0xC1:      //POP B
        {
        state.C= read_memory(state.sp);    // pop lower part of register
        state.B = read_memory(state.sp+1);  // pop upper part of registervoid POP(State* state, uint8_t *register_choice_1, uint8_t *register_choice_2){
        state.sp += 2;
        break;
        }

        case 0xC2:      //JNZ ADDR
        {
            if ((state.cc.z) == 0) {
                JMP();
                opcode_size = 0;
            }
            else{
                opcode_size=3;
            }
            break;
        }

        case 0xC3:      //JMP addr
        {
            JMP();
            opcode_size = 0;
            break;
        }

        case 0xC4:      //CNZ
        {
            if ((state.cc.z) = 0){
                uint16_t address = read_opcode_word();
				uint16_t return_address = state.pc + 3;
				call(address, return_address);
				opcode_size = 0;
            }
            else {
					opcode_size = 3;
				}
            break;
        }

        case 0xC5:      //PUSH B
        {
            write_memory(state.sp-1, state.B);              // save upper part of register on stack -1
            write_memory(state.sp-2, state.C);              // save lower part of register on state -2
            state.sp -= 2; 
            break;
        }

        case 0xC6:      //ADI Byte
        {
            uint16_t answer = (uint16_t) state.A + (uint16_t) read_memory(state.pc + 1);
            state.cc.set_zsp(answer);    // set zsp
            state.cc.set_byte_cy(answer);  // set carry
            state.A = (uint8_t) answer ;  // convert to 8 bit again
            opcode_size= 2;
            break;
        }

        case 0xC8:      //RZ 
        {
            if ((state.cc.z) == 1){
                ret();
                opcode_size = 0 ; 
            }
            break;
        }

        case 0xC9:      //RET
        {
            ret();
            opcode_size = 0;
            break;
        }

        case 0xCA:      //JZ addr
        {
            if ((state.cc.z) == 1) {
                JMP();
                opcode_size = 0;
            }
            else {
                opcode_size=3;
            }
            break;
        }

        case 0xCC:      //CZ
        {
            if ((state.cc.z) == 1){
                uint16_t address = read_opcode_word();
				uint16_t return_address = state.pc + 3;
				call(address, return_address);
				opcode_size = 0;
            }
            else {
					opcode_size = 3;
				}
            break;
        }

        case 0xCD:      //CALL
        {
            
            uint16_t address = read_opcode_word();
            uint16_t return_address = state.pc + 3;
            call(address, return_address);
            opcode_size = 0;
            break;
        }

        case 0xCE:      //ACI D8
        {
            uint16_t answer = (uint16_t) state.A + (uint16_t) read_memory(state.pc + 1)+ state.cc.cy;
            state.cc.set_zsp(answer);    // set zsp
            state.cc.set_byte_cy(answer);  // set carry
            state.A = (uint8_t) answer ;  // convert to 8 bit again
            opcode_size= 2;
            break;
        }

        case 0xD0:      //RNC
        {
        if ((state.cc.cy) == 0) {
            ret();
            opcode_size= 0;
            break;
            }
        }      

        case 0xD1:      //POP D      
        {
        state.E= read_memory(state.sp);    // pop lower part of register
        state.D = read_memory(state.sp+1);  // pop upper part of registervoid POP(State* state, uint8_t *register_choice_1, uint8_t *register_choice_2){
        state.sp += 2;
        break;
        }

        case 0xD2:      //JNC
        {
            if ((state.cc.cy) == 0) {
                JMP();
                opcode_size = 0;
            }
            else{
                opcode_size=3;
            }
            break;
        }

        case 0xD3:      // OUT D8
        {
           // NOT IMPLEMENTED
        }

        case 0xD4:      //CNC
        {
            if ((state.cc.cy) == 0){
                uint16_t address = read_opcode_word();
				uint16_t return_address = state.pc + 3;
				call(address, return_address);
				opcode_size = 0;
            }
            else {
				opcode_size = 3;
			}
            break;
        }

        case 0xD5:      // PUSH D
        {
            write_memory(state.sp-1, state.D);              // save upper part of register on stack -1
            write_memory(state.sp-2, state.E);              // save lower part of register on state -2
            state.sp -= 2; 
            break;
        }

        case 0xD6:      //SUI D8
        {
            uint16_t answer = (uint16_t) state.A - (uint16_t) read_memory(state.pc + 1);
            state.cc.set_zsp(answer);    // set zsp
            state.cc.set_byte_cy(answer);  // set carry
            state.A = (uint8_t) answer ;  // convert to 8 bit again
            opcode_size= 2;
            break;
        }

        case 0XD8:      //RC
        {
        if ((state.cc.cy) == 1) {
            ret();
            opcode_size= 0;
            break;
            }
        } 

        case 0xDA:      //JC   
        {
            if ((state.cc.cy) == 1) {
                JMP();
                opcode_size = 0;
            }
            else{
                opcode_size=3;
            }
            break;
        }

        case 0xDB:      //IN D8
        {
            //NOT IMPLEMENTED
        }
        
        case 0xDC:      //CC
        {
            if ((state.cc.cy) == 1){
                uint16_t address = read_opcode_word();
				uint16_t return_address = state.pc + 3;
				call(address, return_address);
				opcode_size = 0;
            }
            else {
				opcode_size = 3;
			}
            break;
        }

        case 0xDE:      //SBI D8
        {
            uint16_t answer = (uint16_t) state.A - (uint16_t) read_memory(state.pc + 1)- state.cc.cy;
            state.cc.set_zsp(answer);    // set zsp
            state.cc.set_byte_cy(answer);  // set carry
            state.A = (uint8_t) answer ;  // convert to 8 bit again
            opcode_size= 2;
            break;
        }

        case 0xE0:      //RPO
        {
            if ((state.cc.p) == 0) {
                ret();
                opcode_size = 0;
            }
            break;
        }

        case 0xE1:      //POP
        {
            state.L= read_memory(state.sp);    // pop lower part of register
            state.H = read_memory(state.sp+1);  // pop upper part of registervoid POP(State* state, uint8_t *register_choice_1, uint8_t *register_choice_2){
            state.sp += 2;
            break;
        }

        case 0xE2:      //JPO 
        {
        if ((state.cc.p) == 0) {
                JMP();
                opcode_size = 0;
            }
            else{
                opcode_size=3;
            }
            break;
        }
        

        case 0xE3:      //XHTL
        {
            uint8_t l = state.L;
            uint8_t h = state.H;
            state.L = read_memory(state.sp);
            state.H = read_memory(state.sp)+1;
            write_memory(state.sp, l);
            write_memory(state.sp+1,h);
            break;
        }

        case 0xE4:      //CPO
        {
            if ((state.cc.p) == 0){
                uint16_t address = read_opcode_word();
				uint16_t return_address = state.pc + 3;
				call(address, return_address);
				opcode_size = 0;
            }
            else {
				opcode_size = 3;
			}
            break;
        }

        case 0xE5:      //PUSH H

        {
            write_memory(state.sp-1, state.H);              // save upper part of register on stack -1
            write_memory(state.sp-2, state.L);              // save lower part of register on state -2
            state.sp -= 2; 
            break;
        }

        case 0xE6:      //ANI D8
        {
            uint16_t value = uint16_t(state.A) & uint16_t(read_memory(state.pc + 1));
            state.cc.set_byte_cy(value);
            state.cc.set_zsp(value);
            state.A = (uint8_t) value ;
            opcode_size = 2;
            break;
        }

        case 0xE8:      //RPE
        {
            if (state.cc.p == 1) {
				ret();
				opcode_size = 0;
				}
            break;
  
        }

        case 0xE9:      //PCHL
    {
        state.pc = state.read_reg(&state.H, &state.L);   // jumps to address in HL register
        opcode_size = 0;
        break;
    }

    case 0xEA:      // JPE adr
    {
         if ((state.cc.p) == 1) {
                JMP();
                opcode_size = 0;
            }
            else{
                opcode_size=3;
            }
            break;
    }

    case 0xEB:      //XCHG
    {
        uint8_t l = state.L;
        uint8_t h = state.H;
        state.L = read_memory(state.D);
        state.H = read_memory(state.E);
        write_memory(state.D, l);
        write_memory(state.E, h);
        break;
    }

    case 0xEC:      //CPE
    {
         if ((state.cc.p) == 1){
                uint16_t address = read_opcode_word();
				uint16_t return_address = state.pc + 3;
				call(address, return_address);
				opcode_size = 0;
            }
            else {
					opcode_size = 3;
				}
            break;
    }

    case 0xEE:      //XRI D8
    {
            uint16_t value = uint16_t(state.A) ^ uint16_t(read_memory(state.pc + 1));
            state.cc.set_byte_cy(value);
            state.cc.set_zsp(value);
            state.A = (uint8_t) value ;
            opcode_size = 2;
            break;
    }

    case 0xF0:      //RP
    {
        if ((state.cc.s) == 0) {
            ret();
            opcode_size = 0;
        }
        break;
    }

    case 0xF1:      //POP PSW
    {
        uint8_t psw = read_memory(state.sp);
        state.cc = *reinterpret_cast<ConditionCodes*>(&psw);
        state.A = read_memory(state.sp + 1);
        state.sp += 2;
        break;
    }

    case 0xF2:      //JP 
    {
        if ((state.cc.p) == 1) {
                JMP();
                opcode_size = 0;
        }
            else {
                opcode_size=3;
            }
        break;
    }

    case 0xF3:      //DI
    {
        state.InterruptEnabled = false; 
        break;
    }

    case 0xF4:      //CP
    {
        if ((state.cc.s) == 0){
            uint16_t address = read_opcode_word();
            uint16_t return_address = state.pc + 3;
            call(address, return_address);
            opcode_size = 0;
        }
        else {
            opcode_size = 3;
        }
        break;
    }

    case 0xF5:      //PUSH PSW
    {
        write_memory(state.sp-2,*reinterpret_cast<uint8_t*>(&state.cc));
        write_memory(state.sp-1,state.A);
        state.sp -= 2;
        break;
    }

    case 0xF6:      //ORI D8
        {
            uint16_t value = uint16_t(state.A) | uint16_t(read_memory(state.pc + 1));
            state.cc.set_byte_cy(value);
            state.cc.set_zsp(value);
            state.A = (uint8_t) value ;
            opcode_size = 2;
            break;
    }

    case 0xF8:      //RM
    {
        if ((state.cc.s) == 1) {
            ret();
            opcode_size = 0;
        }
        break;
    }

    case 0xF9:      //SPHL
    {
    state.sp = state.read_reg(&state.H, &state.L);
    break;
    }

    case 0xFA:      //JM adr
    {
         if ((state.cc.s) == 1) {
                JMP();
                opcode_size = 0;
            }
            else{
                opcode_size=3;
            }
        break;
    }

    case 0xFB:      //EI
    { 
        state.InterruptEnabled = true;   
        break;
    }

    case 0xFC:      //CM
    {
        if ((state.cc.s) == 1){
            uint16_t address = read_opcode_word();
            uint16_t return_address = state.pc + 3;
            call(address, return_address);
            opcode_size = 0;
        }
        else {
            opcode_size = 3;
        }
        break;
    }

    case 0xFE:      //CPI D8
    {
            uint16_t value = uint16_t(state.A) - uint16_t(read_memory(state.pc + 1));
            state.cc.set_byte_cy(value);
            state.cc.set_zsp(value);
            opcode_size = 2;
            break;
    }


}


state.pc += opcode_size;

}

