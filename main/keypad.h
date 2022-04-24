#ifndef _KEYPAD_H
#define _KEYPAD_H

#include "pindef.h"
#include "ui.h"

#define BTN_HOLD_TIME 100
#define ROWS 4 
#define COLS 4

enum KeypadButton {
  ZERO='0', ONE, TWO, THREE, FOUR, FIVE,
  SIX, SEVEN, EIGHT, NINE,
  UP='u', DOWN='d',
  BACK='b', OK='e',
  DOT='.', MODE_E='E',
};

enum ButtonState {
  BTN_PRESSED,
  BTN_RELEASED,
  BTN_HELD,
};

typedef struct {
  ButtonState state;
  KeypadButton btn;
  int holdTime;
} Button;

extern void (*btn_held_fn)(char);

extern Button buttons[ROWS][COLS];
extern int newButtonInput;

const byte rowPinsDef[] = {KEYPAD_R1, KEYPAD_R2, KEYPAD_R3, KEYPAD_R4}; 
const byte colPinsDef[] = {KEYPAD_C1, KEYPAD_C2, KEYPAD_C3, KEYPAD_C4};

void keypad_init();
char getkey();
char processKeypadInput();
void readKeypad();

#endif
