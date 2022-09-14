#include "State.h"
#include <stdint.h>

State::State(): 
A(0), B(0), C(0), D(0), E(0), H(0), L(0), pc(0), sp(0), memory{0}, InterruptEnabled(false) {

};

void State::reset(){

    cc.reset();
    A = 0;
    B = 0;
    C = 0;
    D = 0;
    H = 0;
    L = 0;
    pc = 0;
    sp = 0;

    InterruptEnabled = false;


};

uint16_t State::update_reg(uint8_t* regA, uint8_t* regB) {

	uint16_t value = (*regA <<8) | (*regB) ; 
	return value;
}