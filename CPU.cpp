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

State& CPU::get_state(){
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
    state.pc = jmp_addr;                                    // put 16 bit address into PC
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
 // ADD to A in one of three modes, option for carry: encompasses ADD, ADC, ADI 
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

void CPU::step(){
    num_steps+=1;
    int opcode_size = 1;
    uint8_t opcode = read_memory(state.pc);
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
        }

        case 0x39:      //DAD SP
        {
            uint16_t HL = state.read_reg(&state.H, &state.L); //convert to 32 bits so that 16 bit register addition saves the carry
            uint32_t answer = (uint32_t) HL + (uint32_t) state.sp;
            state.H = state.write_reg_A((uint16_t*) &answer);
            state.L = state.write_reg_B((uint16_t*) &answer);
            state.cc.set_word_cy(answer);
        }

        case 0x3a:      //LDA ADDR
        {
            uint16_t addr = read_opcode_word();
            state.A = read_memory(addr);
            opcode_size = 3;
        }

        case 0x3b:      //DCX SP
        {
            state.sp--;
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
        }

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

        case 0x57:
        {
            MOV(&state.D, &state.A, REG);
            break;
        }

        case 0x58:      //MOV E,B
        {
            break;
            MOV(&state.E, &state.B, REG);
        }

        case 0x59:      //MOV E,D
        {
            break;
            MOV(&state.E, &state.C, REG);
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

        case 0x5F:
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
        }

        case 0x80:
        {
            ADD(&state.B, REG, false );
        }
    }
}
