#include "oled.h"
#include "util.h"
#include "spi.h"
#include <util/delay.h>


static inline void oledEnableWrite( OLED *oled ) {
	uint8_t command[] = {0x5C};
	oledSendCommand( oled, command, 1 );
}

OLED oledInit( unsigned csPin, unsigned dcPin, unsigned resPin ) {
	if( SPI_INITIALIZED ) {
		spiAddDevice( csPin );	
	} else {
		spiMasterInit( csPin );
	}	

	// Enable output for Data/Command and Reset pin
	digitalPinMode( dcPin, OUTPUT );
	digitalPinMode( resPin, OUTPUT );

	// Reset Display on startup
	digitalWrite( resPin, LOW );
	_delay_ms(1);
	digitalWrite( resPin, HIGH );	

	OLED oled = {.cs=csPin, .dc=dcPin, .res=resPin};

	// Unlock OLED command driver
	uint8_t unlockCommand[] = {0xFD, 0x12};
	oledSendCommand( &oled, unlockCommand, 2 );

	// Set color depth
	uint8_t colorDepthCommand[] = {0xA0, 0xA0};
	oledSendCommand( &oled, colorDepthCommand, 2 );

	return oled;
}

void oledEnableDisplay( OLED *oled, unsigned value ) {
	uint8_t displayCommand[1];
	if( value ) { // Turn display on
		displayCommand[0] = 0xAF;
	} else { // Turn display off
		displayCommand[0] = 0xAE;
	}
	oledSendCommand( oled, displayCommand, 1 );
}

void oledSendCommand( OLED *oled, uint8_t *command, uint8_t bytes ) {
	spiSetDataOrder( MSB );
	digitalWrite( oled->dc, LOW );
	spiWriteByte( oled->cs, command[0] );	
	digitalWrite( oled->dc, HIGH );
	
	for( uint8_t i = 1; i < bytes; ++i ) {
		spiWriteByte( oled->cs, command[i] );	
	}
}

void oledDrawPixel( OLED *oled, uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b ) {
	oledDrawRect( oled, x, y, 1, 1, r, g, b );
}

void oledDrawRect( OLED *oled, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t r, uint8_t g, uint8_t b ) {
	if( x > 127 || y > 127 ) return;

	uint8_t endColumnAddress = (x + width) > 127 ? (x + width) : 127;
	uint8_t endRowAddress = (y + height) > 127 ? (y + height) : 127;
	uint8_t setColumnCommand[] = {0x15, x, endColumnAddress};
	uint8_t setRowCommand[] = {0x75, y, endRowAddress};
	oledSendCommand( oled, setColumnCommand, 3 );
	oledSendCommand( oled, setRowCommand, 3 );
	oledEnableWrite( oled );

	// 262K Color Mode
	for( uint8_t yIndex = 0; yIndex < endRowAddress - y; ++yIndex ) {
		for( uint8_t xIndex = 0; xIndex < endColumnAddress - x; ++xIndex ) {
			spiWriteByte( oled->cs, r );
			spiWriteByte( oled->cs, g );
			spiWriteByte( oled->cs, b );
		}
	}
}

void oledDrawBitMap( OLED *oled, uint8_t x, uint8_t y, uint8_t width, uint8_t height, Pixel *pixBuf ) {
	if( x > 127 || y > 127 ) return;

	uint8_t endColumnAddress = (x + width) > 127 ? (x + width) : 127;
	uint8_t endRowAddress = (y + height) > 127 ? (y + height) : 127;
	uint8_t setColumnCommand[] = {0x15, x, endColumnAddress};
	uint8_t setRowCommand[] = {0x75, y, endRowAddress};
	oledSendCommand( oled, setColumnCommand, 3 );
	oledSendCommand( oled, setRowCommand, 3 );	
	oledEnableWrite( oled );	

	Pixel *pixRow = pixBuf;
	for( uint8_t yIndex = 0; yIndex < endRowAddress - y; ++yIndex ) {
		Pixel *pixColumn = pixRow;
		for( uint8_t xIndex = 0; xIndex < endColumnAddress - x; ++xIndex ) {
			spiWriteByte( oled->cs, pixColumn->red );
			spiWriteByte( oled->cs, pixColumn->green );
			spiWriteByte( oled->cs, pixColumn->blue );
			pixColumn++;
		}
		pixRow += width;
	}
	
}
