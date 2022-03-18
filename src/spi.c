#include "spi.h"

void spiMasterInit() {
	// Set MOSI and SCK as output
	DDRB |= (1 << DDB1) | (1 << DDB2);
	// Enable SPI, Master, set clock rate fck/16
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void spiWriteByte( unsigned csPin, uint8_t byte ) {
	// Start Transmission
	SPDR = byte;
	// Wait for transmission to complete
	while( !(SPSR & (1 << SPIF)) );
}
