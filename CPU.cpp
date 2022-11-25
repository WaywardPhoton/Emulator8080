#include "CPU.h"
#include "helpers.h"

CPU::CPU(): memory_ptr(nullptr){
    reset_num_steps();
    state.reset();
}


void CPU::init(Memory* input_memory, uint16_t pc_start){
    memory_ptr = input_memory;
    state.pc = pc_start;
    reset_num_steps();
}

void CPU::reset_num_steps(){
    num_steps = 0;
}

int& CPU::get_num_steps(){
    return num_steps
}

State& CPU::get_state(){
    return state
}

void CPU::step();
    // generate interrupt
void CPU::interrupt (int interrupt_index){

    uint16_t ret = state.pc +2 ;  // save return address by looking at next instruction pointed to by program counter (CALL is 3 bytes long)
    write_memory(state.sp-1, ((ret >> 8) & 0xFF))                // save upper part of address on stack -1         
    write_memory(state.sp-2,(ret & 0xFF))                       // save lower part of address on stack -2
    state.sp -= 2;                                             //  decrement stack pointer 
    state.pc = 8 * index ;                                   // jump to interrupt subroutine 

}

void CPU::read_opcode_word() const{

    uint16_t opcode = create_word(read_memory(state.pc+2), read_memory(state.pc+1));
    return opcode
}

uint8_t CPU::read_memory(uint16_t address) const {

    uint16_t addr = memory->translate(address);
    uint8_t value = memory->read(addr);
    return value 
}

void CPU::write_memory(uint16_t address ,uint8_t value){

    uint16_t addr = memory -> translate(address);
    memory->write(addr, value);
}

void CPU::call(uint16 jmp_addr, uint16_t ret_addr ){
    write_memory(state->sp-1, ((ret_addr >> 8) & 0xFF));     // save upper part of address on stack -1
    write_memory(state->sp-2, (ret_addr & 0xFF));            // save lower part of address on stack -2
    state.sp -= 2;                                           //  decrement stack pointer to point to new bottom of stack (PUSH)
    state-.pc = jmp_addr;                                    // put 16 bit address into PC
}

void CPU::ret (){
    uint16_t ret_addr= create_word(read_memory(state.sp+1), read_memory(state.sp));
    state.pc = ret_addr;                          // pull address from 8 bit memory locations based on SP and SP+1
    state.sp +=2;                                                               // increment stack pointer to point to new bottom of stack (POP)
}


void CPU::step(){
    num_steps+=1;
    int opcode_size = 1;
    opcode = read_memory(state.pc);
    switch(opcode):
    {
        case 0x00: break;                         //NOP
        case 0x01:
        {
            state.B = read_memory(state.pc+2);   // LXI B, D16
            state.C = read_memory(state.pc+1);
            opcode_size = 3;
            break;
        }
        case 0x11:
        {
            state.D = read_memory(state.pc+2);   // LXI D, D16
            state.E = read_memory(state.pc+1);
            opcode_size = 3;
            break; 
        }
        case 0x21:{
            state.H = read_memory(state.pc+2);   // LXI H, D16
            state.L = read_memory(state.pc+1);
            opcode_size = 3;
            break;  
        }
        case 0x31:
        {
            state.sp = read_opcode_word();    // :LXI SP, D16
            opcode_size = 3;

        }
        case 0x02:
        {
            uint16_t address = state->read_reg(state.B,state.C); //STAX B
            write_memory(address, state.A);
        }
        case 0x12:
        {
            uint16_t address = state->read_reg(state.D,state.E); //STAX D
            write_memory(address, state.A);
        }


    }
}