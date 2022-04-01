#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>  
#include <utility/Adafruit_MCP23017.h>

#define WHITE 0x7

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
    
  int timer = 1000;
  lcd.begin(16, 2);
  lcd.setBacklight(WHITE);

  //Serial.print("test");
  //Opening Message
  lcd.setCursor(4,0);
  lcd.print("Welcome!");
  delay(timer);  
  selectionMenu();
}
/*
40 characters before it wraps to new line
Currently displays 16 characters
Have autoscroll after inital print
*/

void printMessage(String message){
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

/*
void selectionMenu (){
  //Selection Menu
  lcd.clear();
  String message ="";
  while (true){
    lcd.clear();
    message = "Locate Resistor";
    printMessage(message);
    delay(1000);
    message =  "Input using Color or Number";
    lcd.setCursor(0,1);
    printMessage(message);
  }
}
*/


void displayNumber(String magnitude, String exponent){
  lcd.clear();
  lcd.setCursor(0,0);
  printMessage("Value:");
  lcd.setCursor(0,1);
  printMessage(magnitude);
  printMessage(exponent);
}


void loop(){
  
  
  }
