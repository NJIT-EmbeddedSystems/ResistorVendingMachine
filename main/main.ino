#include <Arduino.h>

#include "pindef.h"
#include "oled.h"
#include "keypad.h"
#include "ui.h"

void setup() {
    Serial.begin(9600);
    
    keypad_init();
    oled_init();
    state_init();
    
    Serial.println( "Starting Resistor Vending Machine" );
    drawMainMenu();
}

void loop() {
    char input = getkey();
    if( input != 0 ) {
      update_state( input );
      redraw_state();
      Serial.println(input);
    }
    
    /*processInput();
    if( newButtonInput ) {
      update_state();
      redraw_state();
    }*/
  
    delay(10);
}
