#ifndef _PINDEF_H
#define _PINDEF_H

// SPI Pins (Used for OLED/SD Card)
#define MISO_PIN	50
#define MOSI_PIN	51
#define SCLK_PIN	52 

// OLED/SD Card Device Pins
#define DC_PIN      53
#define OLED_CS_PIN 49
#define SD_CS_PIN   6
#define RST_PIN     48

// I2C Pins (Used for LCD) 
#define SDA			20
#define SCL			21

// Keypad pins
// From left to right [NC = no connection]
// NC C1 C2 C3 C4 R1 R2 R3 R4 NC
#define KEYPAD_C1 23
#define KEYPAD_C2 25
#define KEYPAD_C3 27
#define KEYPAD_C4 29

#define KEYPAD_R1 22
#define KEYPAD_R2 24
#define KEYPAD_R3 26
#define KEYPAD_R4 28

#endif /* _PINDEF_H */
