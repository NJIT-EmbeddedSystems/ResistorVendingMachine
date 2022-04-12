#include "Arduino.h"
#include "SetColor.h"

int drawerNum = 0;
int moduleNum = 0;

void setup(){
    colorInit();
    
};

void loop(){
    ledOn(moduleNum, drawerNum);
    setRed();
    delay(5000);
    setGreen();
    delay(5000);
    setYellow();
    delay(5000);
    ledOff();
    delay(5000);

};