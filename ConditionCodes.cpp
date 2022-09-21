#include "ConditionCodes.h"
# include "helpers.h"
#include <stdio.h>


ConditionCodes::ConditionCodes() {
    all = 0; 
}

void ConditionCodes::reset() {
		all = 0;
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



