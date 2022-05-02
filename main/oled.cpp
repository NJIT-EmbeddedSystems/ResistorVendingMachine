#include "oled.h"

#include <Adafruit_SSD1351.h>
#include <Adafruit_GFX.h>
#include <SPI.h>

#include "pindef.h"

Adafruit_SSD1351 *oled;
static int oled_initialized = 0;
bool oledDisplayOn;

void oled_init() {
	if( oled_initialized ) return;

	oled = new Adafruit_SSD1351( OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGHT, &SPI, OLED_CS_PIN, DC_PIN, RST_PIN );
	oled_initialized = 1;
	oled->begin();
	oled->setFont();
	oled->setTextSize( 1 );
	oled->enableDisplay( 1 );
  oledDisplayOn = true;
}

void oled_set_display( bool on ) {
  oled->enableDisplay( on );
  oledDisplayOn = on;
}

bool oled_is_display_on() {
  return oledDisplayOn;
}

void oled_set_rotation( enum OledRotation rotation ) {
	oled->setRotation( rotation );
}
