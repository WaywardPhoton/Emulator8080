#include "ConditionCodes.h"
# include "helpers.h"
#include <stdio.h>


ConditionCodes::ConditionCodes() {
    all = 0; 
}

void ConditionCodes::reset() {
		all = 0;
        padding_2 = 1;
	}

void ConditionCodes::set_zsp(uint16_t value) {
		z = ((value & 0xff) == 0);
        s = ((value & 0x80) != 0);
        p = ParityCheck(value & 0xff);

        
    }

void ConditionCodes::set_byte_cy(uint16_t value) {
        cy = ((value > 0xff) != 0);
        
    }

void ConditionCodes::set_word_cy(uint32_t value) {
        cy = ((value > 0xffff) != 0);
        
    }

void ConditionCodes::set_aux_carry(uint8_t a, uint8_t b, uint8_t c)
        {
            ac = ((a & 0x0f) + (b & 0x0f) + (c & 0x0f)) > 0x0f;
        }

void ConditionCodes::set_aux_carry(uint8_t a)
        {
            ac = ((a & 0xf) == 0xf);
        }



