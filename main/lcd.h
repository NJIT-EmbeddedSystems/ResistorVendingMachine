#ifndef _LCD_H
#define _LCD_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>  
#include <utility/Adafruit_MCP23017.h>
#define WHITE 0x7

/*
40 characters before it wraps to new line
Currently displays 16 characters
Have autoscroll after inital print
*/
class LCD{
  private:
    Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
  public:
     LCD(){
        lcd.begin(16, 2);
        lcd.setBacklight(WHITE);

        // Create Ohm Character
        /*uint8_t ohm[] = {0x0,0xe,0x11,0x11,0x11,0xa,0x1b};
        lcd.createChar( 0, ohm );
        lcd.clear();*/
      }
    void writeStr(String message){
      if (message.length() > 16){    
  
      lcd.print(message.substring(0,16));
      delay (1500);
    
      lcd.autoscroll();
    
      for (int i = 16; i < message.length(); ++i){
        lcd.print(message.charAt(i));   
        delay (500);
      } 
     
      delay(1000);

      lcd.noAutoscroll();   
    }
  
    else{
      lcd.print(message);
      }
    }

    void clearDisplay() {
      lcd.clear();
      lcd.setCursor(0,0);
    }
    
   void displayNumber(String magnitude, String exponent){
    lcd.clear();
    lcd.setCursor(0,0);
    this->writeStr("Value:");
    lcd.setCursor(0,1);
    writeStr(magnitude);
    if( exponent.length() > 0 )
      writeStr("e");
    writeStr(exponent);
    char ohm = 244;
    if( magnitude.length() > 0 )
      writeStr((String)ohm);
  } 
  void line(int lineNumber){
    lcd.setCursor(0,lineNumber);
    }

  void displayResistorCheckout(String magnitude, String exponent, String amount) {
    displayNumber( magnitude, exponent );
    lcd.setCursor(9, 0);
    writeStr("Amount:");
    lcd.setCursor(16-amount.length(), 1);
    writeStr( amount );
  }
};

extern LCD *lcd;
void lcd_init();

#endif
