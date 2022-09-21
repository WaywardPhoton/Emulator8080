#include "State.h"

void ADD_A (State* state, unsigned char *instruction, uint8_t *dest, AddressingMode mode, bool carrybool);

void SUB_A (State* state, unsigned char *instruction, uint8_t *dest, AddressingMode mode, bool carrybool);

void INX (uint8_t *register_choice_1, uint8_t *register_choice_2);

void DCX (uint8_t *register_choice_1, uint8_t *register_choice_2);

void INR (State* state, uint8_t *register_choice);

void DCR (State* state, uint8_t *register_choice, RegisterChoice choice);
