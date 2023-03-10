#include <stdint.h>
#include <cstddef>
#include <stdio.h>
#include "Memory.h"
#include "State.h"
class CPU
{
public:
    CPU(); //constructor for the class 

    // initialize emulator
    void init (Memory* memory, uint16_t pc_start);
    //reset number of steps to zero
    void reset_num_steps();
    // get number of steps in ROM simulated
    int& get_num_steps();
    // get the current state struct
    State get_state();   // experiment with making this const later? 
    // advance through ROM
    void step(FILE* fp);
    // generate interrupt
    void interrupt (int interrupt_index);
    uint8_t read_memory(uint16_t address) const ;

//private:
    uint16_t read_opcode_word() const ;
    void write_memory(uint16_t address ,uint8_t value);
    void call(uint16_t jmp_addr, uint16_t ret_addr);
    void ret ();
    int num_steps; 
    Memory* memory;  //ptr to an instance of the memory class
    State state; 
// private
private:
    void ADD(uint8_t *dest, AddressingMode mode, bool carrybool);
    void SUB(uint8_t *dest, AddressingMode mode, bool carrybool);
    void MOV(uint8_t *register_choice_1, uint8_t *operand_2, AddressingMode mode);
    void AND(uint8_t *dest, AddressingMode mode);
    void ORR(uint8_t *dest, AddressingMode mode);
    void XOR(uint8_t *dest, AddressingMode mode);
    void CMP(uint8_t *dest, AddressingMode mode);
    void JMP();

};
