#include <stdint.h>

struct ConditionCodes{
    ConditionCodes();

    void reset();
    void set_zspcy(uint16_t value);


    union{
    struct{    
    uint8_t z:1;
    uint8_t s:1;
    uint8_t p:1;
    uint8_t cy:1;


    };
    uint8_t all; 
};
};




