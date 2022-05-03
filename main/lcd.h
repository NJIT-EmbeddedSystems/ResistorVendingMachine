#ifndef _LCD_H
#define _LCD_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>  
#include <utility/Adafruit_MCP23017.h>
#define LCD_BACKLIGHT_WHITE 0x7
#define LCD_BACKLIGHT_OFF 0x0

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
        lcd.setBacklight(LCD_BACKLIGHT_WHITE);

        // Create Ohm Character
        /*uint8_t ohm[] = {0x0,0xe,0x11,0x11,0x11,0xa,0x1b};
        lcd.createChar( 0, ohm );
        lcd.clear();*/
      }

    void setDisplay( bool on ) {
      if( on ) {
        lcd.setBacklight(LCD_BACKLIGHT_WHITE);
        lcd.display();
      } else {
        lcd.setBacklight(LCD_BACKLIGHT_OFF);
        lcd.noDisplay();
      }
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
    lcd.clear();
    lcd.setCursor(0,0);
    this->writeStr("Value:");
    lcd.setCursor(0,1);
    writeStr(magnitude);
    int exponentValue = exponent.toInt();
    if(exponentValue == 0 ) {
      
    } else if(exponentValue == 3) {
      writeStr("k");
    } else if (exponentValue == 6) {
      writeStr("M");
    } else {
      writeStr("e");
      writeStr((String)exponent.charAt(0));
    }
    char ohm = 244;
    writeStr((String)ohm);
    
    //displayNumber( magnitude, exponent );
    lcd.setCursor(9, 0);
    writeStr("Amount:");
    lcd.setCursor(16-amount.length(), 1);
    writeStr( amount );
  }

  void displayNotInStock(String magnitude, String exponent) {
    lcd.clear();
    lcd.setCursor(0, 0);
    writeStr("Closest in Stock");
    lcd.setCursor(0, 1);
    writeStr(magnitude);
    int exponentValue = exponent.toInt();
    if(exponentValue == 0 ) {
      
    } else if(exponentValue == 3) {
      writeStr("k");
    } else if (exponentValue == 6) {
      writeStr("M");
    } else {
      writeStr("e");
      writeStr((String)exponent.charAt(0));
    }
    char ohm = 244;
    writeStr((String)ohm);
  }
};

extern LCD *lcd;
void lcd_init();

#endif
