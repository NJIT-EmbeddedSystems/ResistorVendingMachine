#include "Arduino.h"
#include "SetColor.h"
#include "pindef.h"

bool isLedOn;

void colorInit(){
  isLedOn = false;
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
  pinMode(R_CTRL, OUTPUT);
  pinMode(G_CTRL, OUTPUT);
  pinMode(B_CTRL, OUTPUT);
}

void ledOn(int moduleNum, int drawerNum){
    ledOff();
    isLedOn = true;
    // select proper drawer (LED) by updating slave's selects
    /*Serial.print("Setting slave selects to ");
    Serial.println(drawerNum);*/
    digitalWrite(SLAVE_S2, (drawerNum>>2) & 1);
    digitalWrite(SLAVE_S1, (drawerNum>>1) & 1);
    digitalWrite(SLAVE_S0, (drawerNum>>0) & 1);

    // select proper slave decoder
    /*Serial.print("Setting master selects to ");
    Serial.println(moduleNum);*/
    digitalWrite(MASTER_S2, (moduleNum>>2) & 1);
    digitalWrite(MASTER_S1, (moduleNum>>1) & 1);
    digitalWrite(MASTER_S0, (moduleNum>>0) & 1);
    setGreen();

    // turn on the master decoder, allows LED to come on
    //digitalWrite(MASTER_EN, HIGH);

}

void ledOff(){
    isLedOn = false;
    // turn of master decoder, which turns off all LEDs
    //Serial.println("Turn all LED off");
    //digitalWrite(MASTER_EN, LOW);
    analogWrite(R_CTRL, 255);
    analogWrite(G_CTRL, 255);
    analogWrite(B_CTRL, 255);
}

void setGreen(){
    //Serial.println("Setting color green");
    analogWrite(R_CTRL, 255); // R is fully off
    analogWrite(G_CTRL, 0); // G is fully on
    analogWrite(B_CTRL, 255); // B is fully off
}
void setYellow(){
    //Serial.println("Setting color yellow");
    analogWrite(R_CTRL, 120); // R  overpowers so 120 makes yellow
    analogWrite(G_CTRL, 0); // G is fully on
    analogWrite(B_CTRL, 255); // B is fully off
}
void setRed(){
    //Serial.println("Setting color red");
    analogWrite(R_CTRL, 0); // R is fully on
    analogWrite(G_CTRL, 255); // G is fully off
    analogWrite(B_CTRL, 255); // B is fully off
}
