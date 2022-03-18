#ifndef _SPI_H
#define _SPI_H

#include <avr/io.h>

void spiMasterInit();

void spiWriteByte( unsigned csPin, uint8_t byte );

#endif /* _SPI_H */
