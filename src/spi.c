#include "spi.h"
#include "util.h"

unsigned SPI_INITIALIZED = 0;

void spiMasterInit( unsigned csPin ) {
	spiAddDevice( csPin );
	// Set MOSI and SCK as output
	DDRB |= (1 << DDB1) | (1 << DDB2);
	// Enable SPI, Master, set clock rate fck/16
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
	SPI_INITIALIZED = 1;
}

void spiAddDevice( unsigned csPin ) {
	digitalPinMode( csPin, OUTPUT );
	digitalWrite( csPin, HIGH );
}

void spiWriteByte( unsigned csPin, uint8_t byte ) {
	digitalWrite( csPin, LOW );
	// Start Transmission
	SPDR = byte;
	// Wait for transmission to complete
	while( !(SPSR & (1 << SPIF)) );
	digitalWrite( csPin, HIGH );
}

uint8_t spiReadByte( unsigned csPin ) {
	digitalWrite( csPin, LOW );	
	uint8_t message = SPDR;
	digitalWrite( csPin, HIGH );
	return message;
}

void spiSetDataOrder( enum SpiDataOrder order ) {
	if( order == LSB ) {
		SPCR |= (1 << DORD);
	} else {
		SPCR &= ~(1 << DORD);
	}
}
