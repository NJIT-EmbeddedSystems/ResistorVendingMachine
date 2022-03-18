#ifndef _UTIL_H
#define _UTIL_H

#include <avr/io.h>

enum PinMode {
    INPUT,
	INPUT_PULLUP,
    OUTPUT,
};

enum PinType {
    ANALOG,
    DIGITAL,
};

enum PinProps {
    PULL_UP,
    PULL_DOWN,
};

#define HIGH 1
#define LOW  0

void nop();

void pinMode( unsigned pin, enum PinType type, enum PinMode mode );
void digitalPinMode( unsigned pin, enum PinMode mode );
void analogPinMode( unsigned pin, enum PinMode mode );

unsigned digitalPinToHardware( unsigned pin );
unsigned analogPinToHardware( unsigned pin );

void digitalWrite( unsigned pin, unsigned value );
unsigned  digitalRead( unsigned pin );

void analogWrite( unsigned pin, uint8_t value );
unsigned  analogRead( unsigned pin );

#endif /* _UTIL_H */
