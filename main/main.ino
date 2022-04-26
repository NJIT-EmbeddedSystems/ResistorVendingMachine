#include <Arduino.h>

#include "pindef.h"
#include "oled.h"
#include "keypad.h"
#include "ui.h"
#include "lcd.h"

void setup() {
    Serial.begin(9600);

    lcd_init();
    keypad_init();
    btn_held_fn = state_btn_held;
    oled_init();
    state_init();
    
    Serial.println( "Starting Resistor Vending Machine" );
    drawMainMenu();
}

void loop() {
    readKeypad();
    char input = processKeypadInput();
    if( input != 0 ) {
      update_state( input);
      redraw_state();
    }
  
    delay(10);
}
