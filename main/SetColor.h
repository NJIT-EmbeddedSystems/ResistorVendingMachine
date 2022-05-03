#ifndef SetColor_h
#define SetColor_h

extern bool isLedOn;

void colorInit();
void ledOn(int moduleNum, int drawerNum);

//Removed unused parameters in below functions
void ledOff();
void setGreen();
void setYellow();
void setRed();

#endif
