#include <stdint.h>
#include "helpers.h"
class Memory
{
public:
    Memory();

    struct Config {
		Config();

		uint16_t sizeRom;
        uint16_t sizeRam;
        bool isRomWriteable;
        bool isRamMirrored;
		};

    uint8_t memory[0xFFFF]; 
    struct Memory::Config get_config();
    void configure(Config& config);
    bool load(const char* kRomFilename, const uint16_t offset_address); 
    uint16_t translate(uint16_t address);

	void write(uint16_t address, uint8_t value);
	uint8_t read(uint16_t address);
	uint16_t size();

private:
    Config config;
};





    

	
