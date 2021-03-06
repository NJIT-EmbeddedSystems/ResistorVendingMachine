#include "oled.h"

#include <Adafruit_SSD1351.h>
#include <Adafruit_GFX.h>
#include <SPI.h>

#include "pindef.h"

static Adafruit_SSD1351 oled;
static int oled_initialized = 0;

void oled_init() {
	if( oled_initialized ) return;

	oled = Adafruit_SSD1351( SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_CS_PIN, DC_PIN, RST_PIN );
	oled_initialized = 1;
	oled.begin();
	oled.setFont();
	oled.setTextSize( 1 );
	oled.enableDisplay( 1 );
}

inline void oled_set_rotation( enum OledRotation rotation ) {
	oled.setRotation( rotation );
}
