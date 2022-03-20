#ifndef _SPI_H
#define _SPI_H

#include <avr/io.h>

enum SpiDataOrder {
	LSB,
	MSB,
};

extern unsigned SPI_INITIALIZED;

void spiMasterInit( unsigned csPin );
void spiAddDevice( unsigned csPin );

void spiWriteByte( unsigned csPin, uint8_t byte );
uint8_t spiReadByte( unsigned csPin );
void spiSetDataOrder( enum SpiDataOrder order ); 

#endif /* _SPI_H */
