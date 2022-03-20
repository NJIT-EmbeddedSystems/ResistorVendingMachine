#ifndef _SDCARD_H
#define _SDCARD_H

#include <avr/io.h>

typedef struct {
	uint8_t cs;
} SDCard;

SDCard sdcardInit( unsigned csPin );


#endif /* _SDCARD_H */
