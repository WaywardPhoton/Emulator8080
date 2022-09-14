#include "ConditionCodes.h"
#include <stdint.h>
#include "helpers.h"
struct State{
    State();
    void reset();
    uint16_t update_reg(uint8_t* regA, uint8_t* regB);
    
    // Register16 BC;
    // Register16 DE;
    // Register16 HL;

    uint8_t A;
    uint8_t B;
    uint8_t C;
    uint8_t D;
    uint8_t E;
    uint8_t H;
    uint8_t L;
    uint16_t pc; // program counter 
    uint16_t sp; //stack pointer
    uint8_t memory[0xFFFF]; 
    uint16_t value;

    ConditionCodes cc ;
    bool InterruptEnabled; 

};

typedef enum AddressingMode{
    REG  = 0,
    IMM = 1,
    ADDR   = 2
} AddressingMode;

typedef enum RegisterChoice{
    A = 0,
    B = 1,
    C = 2,
    D = 3,
    E = 4,
    H = 5,
    L = 6,
    BC = 7,
    DE = 8,
    HL = 9,
    SP = 10


}RegisterChoice; 
