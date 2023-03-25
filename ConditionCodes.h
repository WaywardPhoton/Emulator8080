#include <stdint.h>

struct ConditionCodes{
    ConditionCodes();

    void reset();
    void set_zsp(uint16_t value);
    void set_byte_cy(uint16_t value) ; 
    void set_word_cy(uint32_t value) ; 

    union{ // top down is lsb to msb
    struct{    
    uint8_t cy:1;
    uint8_t padding_2:1;
    uint8_t p:1;
    uint8_t padding_1:1;
    uint8_t ac:1;
    uint8_t padding_0:1;
    uint8_t z: 1;
    uint8_t s:1;



    };
    uint8_t all; 
};
};




