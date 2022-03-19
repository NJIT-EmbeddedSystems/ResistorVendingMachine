#ifndef _OLED_H
#define _OLED_H

#include <avr/io.h>

typedef struct {
	uint8_t cs;  // Chip Select Pin
	uint8_t dc;  // Data/Command Pin
	uint8_t res; // Reset Pin
} OLED;

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} Pixel;

OLED oledInit( unsigned csPin, unsigned dcPin, unsigned resPin );
void oledEnableDisplay( OLED *oled, unsigned value );
void oledSendCommand( OLED *oled, uint8_t *command, uint8_t bytes );
void oledDrawPixel( OLED *oled, uint8_t x, uint8_t y, uint8_t red, uint8_t green, uint8_t blue );
void oledDrawRect( OLED *oled, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t r, uint8_t g, uint8_t b );
void oledDrawBitmap( OLED *oled, uint8_t x, uint8_t y, uint8_t width, uint8_t height, Pixel *pixBuf );

#endif /* _OLED_H */
