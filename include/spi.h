#ifndef _SPI_H
#define _SPI_H

#include <avr/io.h>

void spiMasterInit( unsigned csPin );
void spiAddDevice( unsigned csPin );

void spiWriteByte( unsigned csPin, uint8_t byte );

#endif /* _SPI_H */
