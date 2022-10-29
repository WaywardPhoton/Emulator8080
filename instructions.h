#include "State.h"

void ADD_A (State* state, uint8_t *dest, AddressingMode mode, bool carrybool);

void SUB_A (State* state, uint8_t *dest, AddressingMode mode, bool carrybool);

void INX (State* state, uint8_t *register_choice_1, uint8_t *register_choice_2, RegisterChoice choice);

void DCX (State* state, uint8_t *register_choice_1, uint8_t *register_choice_2, RegisterChoice choice);

void INR (State* state, uint8_t *register_choice, RegisterChoice choice);

void DCR (State* state, uint8_t *register_choice, RegisterChoice choice);

void DAD(State* state, uint8_t *regA, uint8_t *regB );

void DAD_SP(State* state);