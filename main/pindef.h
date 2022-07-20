#ifndef _PINDEF_H
#define _PINDEF_H

// SPI Pins (Used for OLED/SD Card)
#define OLED_MISO_PIN	50
#define OLED_MOSI_PIN	51
#define OLED_SCLK_PIN	52 

// OLED/SD Card Device Pins
#define DC_PIN      53
#define CD_PIN      45
#define OLED_CS_PIN 49
#define SD_CS_PIN   46
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
#define KEYPAD_R4 30

// LED Control
#define MASTER_EN 33 // active HIGH
#define MASTER_S0 5
#define MASTER_S1 6
#define MASTER_S2 7

// the idea is to have ALL slave decoders to share select lines
// because each are only active one at a time
#define SLAVE_S0 10
#define SLAVE_S1 11
#define SLAVE_S2 12

// RGB ctrl pins hooked up to cathodes on LEDs
#define R_CTRL A0
#define G_CTRL A1
#define B_CTRL A2

#endif /* _PINDEF_H */
