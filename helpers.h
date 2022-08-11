#include <stdint.h>

bool ParityCheck(uint16_t value);


struct Register16{
    Register16(uint8_t& RegA, uint8_t& RegB);
    
    uint16_t get();

    //uint8_t& RegA;
    //uint8_t& RegB;
    uint8_t value;
};