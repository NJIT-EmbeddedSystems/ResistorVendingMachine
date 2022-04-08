#include "Arduino.h"
#include "SetColor.cpp"

void colorInit(){
  // initialize the master decorder
  pinMode(MASTER_EN, OUTPUT);
  pinMode(MASTER_S2, OUTPUT);
  pinMode(MASTER_S1, OUTPUT);
  pinMode(MASTER_S0, OUTPUT);

  // intialize slave selects
  pinMode(SLAVE_S2, OUTPUT);
  pinMode(SLAVE_S1, OUTPUT);
  pinMode(SLAVE_S0, OUTPUT);

  // initialize color controls
  pinMode(R_CTRL, INPUT);
  pinMode(G_CTRL, INPUT);
  pinMode(B_CTRL, INPUT);
}

void ledOn(int moduleNum, int drawerNum){
    
    // select proper drawer (LED) by updating slave's selects
    digitalWrite(SLAVE_S2, bitRead(drawerNum,2));
    digitalWrite(SLAVE_S1, bitRead(drawerNum,1));
    digitalWrite(SLAVE_S0, bitRead(drawerNum,0));

    // select proper slave decoder
    digitalWrite(MASTER_S2, bitRead(moduleNum,2));
    digitalWrite(MASTER_S1, bitRead(moduleNum,1));
    digitalWrite(MASTER_S0, bitRead(moduleNum,0));

    // turn on the master decoder, allows LED to come on
    digitalWrite(MASTER_EN, HIGH);

}

void ledOff(int moduleNum, int ledNum){
    // turn of master decoder, which turns off all LEDs
    digitalWrite(MASTER_EN, LOW);
}

void setGreen(int moduleNum, int drawerNum){
    analogWrite(R_CTRL, 255); // R is fully off
    analogWrite(G_CTRL, 0); // G is fully on
    analogWrite(B_CTRL, 255); // B is fully off
}
void setYellow(int moduleNum, int drawerNum){
    analogWrite(R_CTRL, 0); // R is fully off
    analogWrite(G_CTRL, 0); // G is fully on
    analogWrite(B_CTRL, 255); // B is fully off
}
void setRed(int moduleNum, int drawerNum){
    analogWrite(R_CTRL, 0); // R is fully on
    analogWrite(G_CTRL, 255); // G is fully off
    analogWrite(B_CTRL, 255); // B is fully off
}