#include "ConditionCodes.h"
#include <stdint.h>
#include "helpers.h"
struct State{
    
    void reset();

    struct{  

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
    uint16_t PC; // program counter 
    uint16_t SP; //stack pointer

// not implemented: add option to join BC, DE, HL into 16 bit registers

    ConditionCodes cc ;
    };

};

