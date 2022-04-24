#ifndef _KEYPAD_H
#define _KEYPAD_H

#include "pindef.h"

#define rowPins = {KEYPAD_R1, KEYPAD_R2, KEYPAD_R3, KEYPAD_R4}; 
#define colPins = {KEYPAD_C1, KEYPAD_C2, KEYPAD_C3, KEYPAD_C4};

void keypad_init();
char getkey();

#endif