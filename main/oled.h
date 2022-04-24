#ifndef _OLED_H
#define _OLED_H

#include <Adafruit_SSD1351.h>

#define OLED_SCREEN_WIDTH	128
#define OLED_SCREEN_HEIGHT	128

#define OLED_BLACK		0x0000
#define OLED_BLUE		0x001F
#define OLED_RED		0xF800
#define OLED_GREEN		0x07E0
#define OLED_CYAN		0x07FF
#define OLED_MAGENTA	0xF81F
#define OLED_YELLOW		0xFFE0
#define OLED_WHITE		0xFFFF

enum OledRotation {
	CLOCKWISE_90 = 1,
	CLOCKWISE_180 = 2,
	CLOCKWISE_270 = 3,
	COUNTER_CLOCKWISE_90 = 3,
	COUNTER_CLOCKWISE_180 = 2,
	COUNTER_CLOCKWISE_270 = 1,
};

extern Adafruit_SSD1351 *oled;

void oled_init();
void oled_set_timeout( unsigned milliseconds );
void oled_set_rotation( enum OledRotation rotation ); 

void oled_display_resistor( String value );


#endif /* _OLED_H */
