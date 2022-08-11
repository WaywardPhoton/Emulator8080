# include "helpers.h"
#include <stdio.h>


bool ParityCheck(uint16_t value){

    int size = 16;
	int count = 0;

	for (int i = 0; i < size; i++) {
		if (value & 0x01) {
				count += 1;
			}

			value >>= 1;
		}

return (0 == (count % 2));

};

Register16::Register16(uint8_t& regA, uint8_t& regB): value( (regA<<8) | (regB)) {
};

uint16_t Register16::get() {
	return value;
}

// uint16_t Register16::MushRegister(uint8_t& RegA, uint8_t& RegB){
// 	uint16_t offset = (RegA<<8) | (RegB);  
// 	return offset;
// }


