#include <Arduino.h>

#include "pindef.h"
#include "oled.h"
#include "keypad.h"
#include "ui.h"
#include "lcd.h"
#include "SDCard.h"

void setup() {
    Serial.begin(9600);

    lcd_init();
    keypad_init();
    btn_held_fn = state_btn_held;
    oled_init();
    oled_set_rotation(CLOCKWISE_180);
    state_init();
    sdCard_init();
    
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
