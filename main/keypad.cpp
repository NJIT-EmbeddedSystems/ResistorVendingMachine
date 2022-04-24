//#include <Keypad.h>
#include "Adafruit_Keypad.h"
//#define KEYPAD_PID3844
#include "keypad.h"

char hexaKeys[ROWS][COLS] = {
        {'1', '2', '3', 'u'},
        {'4', '5', '6', 'd'},
        {'7', '8', '9', 'b'},
        {'.', '0', 'E', 'e'}
    };

Button buttons[ROWS][COLS];
int newButtonInput;

Adafruit_Keypad* customKeypad;

void keypad_init(){
    customKeypad = new Adafruit_Keypad(makeKeymap(hexaKeys), rowPinsDef, colPinsDef, ROWS, COLS);
    customKeypad->begin();

    newButtonInput = 0;
    for( int i = 0; i < ROWS; ++i ) {
      for( int j = 0; j < COLS; ++j ) {
        buttons[i][j].btn = (KeypadButton)hexaKeys[i][j];
        buttons[i][j].state = BTN_RELEASED;
        buttons[i][j].currentHoldTime = 0;
      }
    }
}
char getkey(){
    customKeypad->tick();
    keypadEvent e = customKeypad->read();
    if( e.bit.EVENT == KEY_JUST_PRESSED ) return (char)e.bit.KEY;
    return 0;
}

void processInput() {
  customKeypad->tick();
  if( customKeypad->available() == 0 ) {
    newButtonInput = 0;
    return;
  }
  newButtonInput = 1;
  while( customKeypad->available() ) {
    keypadEvent e = customKeypad->read();
    if( customKeypad->isPressed( e.bit.KEY ) ) {
      if( buttons[e.bit.ROW][e.bit.COL].state == BTN_PRESSED ) {
        buttons[e.bit.ROW][e.bit.COL].state = BTN_HELD;
      } else if( buttons[e.bit.ROW][e.bit.COL].state == BTN_HELD ) {
        buttons[e.bit.ROW][e.bit.COL].currentHoldTime++;
      }
    } else {
      buttons[e.bit.ROW][e.bit.COL].state = BTN_RELEASED;
      buttons[e.bit.ROW][e.bit.COL].currentHoldTime = 0;
    }
  }

}
