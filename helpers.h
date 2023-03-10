#include <stdint.h>
#include <stdio.h>
#include <cassert>
#include <iostream>
#include <fstream>

bool ParityCheck(uint16_t value);


uint16_t create_word(uint8_t upper, uint8_t lower);

size_t getFileSize(const char* filename) ;