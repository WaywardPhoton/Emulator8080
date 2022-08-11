#include "State.h"
#include <stdint.h>

State::State(): 
A(0), B(0), C(0), D(0), E(0), H(0), L(0), pc(0), sp(0), BC(Register16(B, C)), DE(Register16(D, E)), HL(Register16(H, L)), memory{0}, InterruptEnabled(false) {

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