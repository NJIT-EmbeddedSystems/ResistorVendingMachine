#ifndef SetColor_h
#define SetColor_h
#include "Arduino.h"

#define MASTER_EN 33 // active HIGH
#define MASTER_S0 30
#define MASTER_S1 31
#define MASTER_S2 32

// the idea is to have ALL slave decoders to share select lines
// because each are only active one at a time
#define SLAVE_S0 36
#define SLAVE_S1 37
#define SLAVE_S2 38

// RGB ctrl pins hooked up to cathodes on LEDs
#define R_CTRL 3
#define G_CTRL 4
#define B_CTRL 5


void colorInit();
void ledOn(int moduleNum, int drawerNum);

//Removed unused parameters in below functions
void ledOff();
void setGreen();
void setYellow();
void setRed();

#endif