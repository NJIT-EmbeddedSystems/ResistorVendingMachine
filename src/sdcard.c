#include "sdcard.h"
#include "spi.h"
#include "util.h"

SDCard sdcardInit( unsigned csPin ) {
	SDCard sdcard = {.cs=csPin};
	if( SPI_INITIALIZED ) {
		spiAddDevice( csPin );
	} else {
		spiMasterInit( csPin );
	}

	/* 
		Enable SPI communication with the sd card	
	   	Must set MOSI and CS line to 1 for atleast 74 cycles, then
	   	set CS line to 0 and send the reset command CMD0
	*/
	spiSetDataOrder( MSB );

	digitalWrite( csPin, HIGH );
	for( unsigned i = 0; i < 10; ++i ) {
		SPDR = 0xFF;
		while( !(SPSR & (1 << SPIF) ) );
	}

	uint8_t resetCommand[] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95};
	digitalWrite( csPin, LOW );
	for( unsigned i = 0; i < 7; ++i ) {
		SPDR = resetCommand[i];
		while( !(SPSR & (1 << SPIF) ) );
	}

	/*
		Receive error message
	*/


	return sdcard;
}
