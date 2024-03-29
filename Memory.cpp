#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include "Memory.h"

using namespace std;

Memory::Memory() {
	}

	Memory::Config::Config() : 
		sizeRom(0), sizeRam(0),
		isRomWriteable(true),
		isRamMirrored(false)
	{
	}

	void Memory::configure(Config& inConfig) {
		config = inConfig;
    }

	bool Memory::load(const char* kRomFilename, const uint16_t offset_address){
		uint16_t filesize = (uint16_t) getFileSize(kRomFilename);
		ifstream file;
		file.open(kRomFilename, ios::in | ios::binary);
		if(!file.is_open()){
			return false;
		}
		uint8_t data [filesize];
		
		file.read((char *)data, filesize);
		file.close();

		for(uint16_t i = 0; i < filesize; i++){
			size_t writeAddress = size_t(offset_address) + i;
			memory[writeAddress] = data[i];

	
		}

		return true;

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

