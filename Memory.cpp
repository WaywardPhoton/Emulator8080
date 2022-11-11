#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include "Memory.h"

Memory::Memory() {
	}

	Memory::Config::Config() : 
		sizeRom(0), sizeRam(0),
		isRomWriteable(false),
		isRamMirrored(false)
	{

	}



	void Memory::configure(Config& inConfig) {
		config = inConfig;
    }

	struct Memory::Config Memory::get_config(){
		return config ;
	}

	uint16_t Memory::translate(uint16_t address) {
		uint16_t addr = address;

		if (config.isRamMirrored) {
			while (addr >= size()) {
				addr -= config.sizeRam;
			}
		}

		return addr;
	}


	uint16_t Memory::size() {
		return config.sizeRam + config.sizeRom;
	}


    uint8_t Memory::read(uint16_t address) {
		return memory[address];
	}

    void Memory::write(uint16_t address, uint8_t value){
		if (!config.isRomWriteable) {
			if (address < config.sizeRom) {
				printf("unable to write to address in ROM");
				return;
			}
		}

		memory[address] = value;		
	}
    