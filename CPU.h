#include <stdint.h>
#include "Memory.h"
#include "State.h"
class CPU
{
public:
    CPU();

    // initialize emulator
    void init (Memory* memory, uint16_t pc_start);
    //reset number of steps to zero
    void reset_num_steps();
    // get number of steps in ROM simulated
    int& get_num_steps();
    // get the current state struct
    State& get_state();   // experiment with making this const later? 
    // advance through ROM
    void step();
    // generate interrupt
    void interrupt (int interrupt_index);

private:
    uint16_t read_opcode_word() const ;
    uint8_t read_memory(uint16_t address) const ;
    void write_memory(uint16_t address ,uint8_t value);
    void call(uint16_t input_addr, uint16_t return_addr);
    void ret ();
    int num_steps; 
    Memory* memory_ptr;  //ptr to an instance of the memory class
    State state; 

};
