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
void (*btn_held_fn)(char) = NULL;

Adafruit_Keypad* customKeypad;

void keypad_init(){
    customKeypad = new Adafruit_Keypad(makeKeymap(hexaKeys), rowPinsDef, colPinsDef, ROWS, COLS);
    customKeypad->begin();

    newButtonInput = 0;
    for( int i = 0; i < ROWS; ++i ) {
      for( int j = 0; j < COLS; ++j ) {
        buttons[i][j].btn = (KeypadButton)hexaKeys[i][j];
        buttons[i][j].state = BTN_RELEASED;
        buttons[i][j].holdTime = 0;
      }
    }
}
char getkey(){
    customKeypad->tick();
    keypadEvent e = customKeypad->read();
    if( e.bit.EVENT == KEY_JUST_PRESSED ) return (char)e.bit.KEY;
    return 0;
}

void readKeypad() {
  customKeypad->tick();
  for( int i = 0; i < ROWS; ++i ) {
    for( int j = 0; j < COLS; ++j ) {
      if( customKeypad->justPressed( buttons[i][j].btn, 1 ) ) {
        buttons[i][j].state = BTN_PRESSED;
      } else if( customKeypad->isPressed( buttons[i][j].btn ) ) {
        if(buttons[i][j].holdTime < BTN_HOLD_TIME ) {
          buttons[i][j].holdTime++;
        } else if(buttons[i][j].state == BTN_PRESSED ) {
          buttons[i][j].state = BTN_HELD;
          if( btn_held_fn != NULL )
            btn_held_fn(buttons[i][j].btn);
        }
      } else if( customKeypad->justReleased(buttons[i][j].btn ) ) {
        buttons[i][j].holdTime = 0;
        buttons[i][j].state = BTN_RELEASED;
      }
    }
  }
  
}

char processKeypadInput() {
    customKeypad->tick();
  
    if( customKeypad->available() == 0 ) {
      return 0;
    }
    keypadEvent e = customKeypad->read();
    if( e.bit.EVENT == KEY_JUST_PRESSED ) {
      buttons[e.bit.ROW][e.bit.COL].state = BTN_PRESSED;
      return (char)e.bit.KEY;
    }
    
    /*if( customKeypad->isPressed( e.bit.KEY ) ) {
      if( buttons[e.bit.ROW][e.bit.COL].state == BTN_PRESSED ) {
        if( buttons[e.bit.ROW][e.bit.COL].holdTime < BTN_HOLD_TIME ) {
          buttons[e.bit.ROW][e.bit.COL].holdTime++;
          //Serial.println( buttons[e.bit.ROW][e.bit.COL].holdTime++ );
        } else {
           buttons[e.bit.ROW][e.bit.COL].state = BTN_HELD;
        }
      }
    } else if(e.bit.EVENT == KEY_JUST_RELEASED ) {
      buttons[e.bit.ROW][e.bit.COL].state = BTN_RELEASED;
      buttons[e.bit.ROW][e.bit.COL].holdTime = 0;
    }*/

  return 0;

}
