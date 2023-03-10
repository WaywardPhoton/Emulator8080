# include "helpers.h"



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

}

uint16_t create_word(uint8_t upper, uint8_t lower) {
		return (uint16_t(upper) << 8) | uint16_t(lower);
	}

size_t getFileSize(const char* filename) {
		std::ifstream file;
		file.open(filename, std::ios::in | std::ios::binary | std::ios::ate);
		assert(file.is_open());

		size_t size = static_cast<size_t>(file.tellg());

		file.close();

		return size;
	}