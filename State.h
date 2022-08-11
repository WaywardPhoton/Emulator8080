#include "ConditionCodes.h"
#include <stdint.h>
#include "helpers.h"
struct State{
    State();
    void reset();
    
    Register16 BC;
    Register16 DE;
    Register16 HL;

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

    ConditionCodes cc ;
    bool InterruptEnabled; 

};

typedef enum AddressingMode{
    REG  = 0,
    IMM = 1,
    ADDR   = 2
} AddressingMode;
