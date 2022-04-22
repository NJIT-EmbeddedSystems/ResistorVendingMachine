#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>  
#include <utility/Adafruit_MCP23017.h>
#define WHITE 0x7

void setup() {
  
}
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
    
   void displayNumber(String magnitude, String exponent){
    lcd.clear();
    lcd.setCursor(0,0);
    this->writeStr("Value:");
    lcd.setCursor(0,1);
    writeStr(magnitude);
    writeStr(exponent);
  } 
  void line(int lineNumber){
    lcd.setCursor(0,lineNumber);
    }
};

void loop(){
  
  
  }
