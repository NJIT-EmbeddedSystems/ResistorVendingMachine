#include <Arduino.h>

#include "pindef.h"
#include "oled.h"
#include "keypad.h"
#include "ui.h"
#include "lcd.h"
#include "SDCard.h"
#include "SetColor.h"

unsigned long lastInput;
unsigned long currentTime;
unsigned long oledTimeout;
unsigned long ledIndicatorTimeout;

void setup() {
    Serial.begin(9600);

    lcd_init();
    colorInit();
    ledOff();
    keypad_init();
    btn_held_fn = state_btn_held;
    oled_init();
    //oled_set_rotation(CLOCKWISE_180);
    state_init();
    sdCard_init();

    lastInput = millis();
    oledTimeout = 20000;
    ledIndicatorTimeout = 10000;
    
    Serial.println( "Starting Resistor Vending Machine" );

    /*while (resistorInventoryFile.available()) {
      Serial.write(resistorInventoryFile.read());
    }*/
    sd_goto_start();
    
    Serial.println( "READING SDCARD" );
    
    do {
      Serial.print( sd_read_resistor_value() );
      Serial.print( "," );
      String magnitude, exponent;
      sd_read_resistor_color_code( &magnitude, &exponent );
      Serial.print( magnitude );
      Serial.print( " " );
      Serial.print( exponent );
      Serial.print( "," );
      Serial.print( sd_read_inventory_count() );
      Serial.print( "," );
      Serial.print( sd_read_module_num() );
      Serial.print( "," );
      Serial.println( sd_read_drawer_num() );
    } while( sd_next_line() );
    Serial.println( "END OF READING SDCARD" );
    sd_goto_start();
    redraw_state();
}

void loop() {
    currentTime = millis();
    if( currentTime - lastInput >= oledTimeout ) {
      oled_set_display( 0 );
      lcd->setDisplay(0);
    }
    if( isLedOn && (currentTime - lastInput >= ledIndicatorTimeout) ) {
      ledOff();
      resetAllStates();
      redraw_state();
    }
  
    readKeypad();
    char input = processKeypadInput();
    if( input != 0 ) {
      lastInput = millis();
      if( !oled_is_display_on() ) {
        oled_set_display( 1 );
        lcd->setDisplay(1);
      } else {
        update_state( input);
        redraw_state();
      }
    }
  
    delay(10);
}
