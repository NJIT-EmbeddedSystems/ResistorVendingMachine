#include "util.h"
#include <avr/io.h>

typedef struct {
	uint16_t reg;
    uint16_t port;
    uint16_t bitOffset;
} PinMap;

static PinMap pinMapTable[] = { // _Pin Map Table for atmega2560
    {0,0,0}, // PADDING
    {(uint16_t) &DDRG, (uint16_t) &PORTG,PG5},{(uint16_t) &DDRE, (uint16_t) &PORTE,PE0},{(uint16_t) &DDRE, (uint16_t) &PORTE,PE1},{(uint16_t) &DDRE, (uint16_t) &PORTE,PE2},{(uint16_t) &DDRE, (uint16_t) &PORTE,PE3},{(uint16_t) &DDRE, (uint16_t) &PORTE,PE4},{(uint16_t) &DDRE, (uint16_t) &PORTE,PE5},{(uint16_t) &DDRE, (uint16_t) &PORTE,PE6},{(uint16_t) &DDRE, (uint16_t) &PORTE,PE7},
    {0,0,0}, // VCC
    {0,0,0}, // GND
    {(uint16_t) &DDRH, (uint16_t) &PORTH,PH0},{(uint16_t) &DDRH, (uint16_t) &PORTH,PH1},{(uint16_t) &DDRH, (uint16_t) &PORTH,PH2},{(uint16_t) &DDRH, (uint16_t) &PORTH,PH3},{(uint16_t) &DDRH, (uint16_t) &PORTH,PH4},{(uint16_t) &DDRH, (uint16_t) &PORTH,PH5},{(uint16_t) &DDRH, (uint16_t) &PORTH,PH6},
    {(uint16_t) &DDRB, (uint16_t) &PORTB,PB0},{(uint16_t) &DDRB, (uint16_t) &PORTB,PB1},{(uint16_t) &DDRB, (uint16_t) &PORTB,PB2},{(uint16_t) &DDRB, (uint16_t) &PORTB,PB3},{(uint16_t) &DDRB, (uint16_t) &PORTB,PB4},{(uint16_t) &DDRB, (uint16_t) &PORTB,PB5},{(uint16_t) &DDRB, (uint16_t) &PORTB,PB6},{(uint16_t) &DDRB, (uint16_t) &PORTB,PB7},
    {(uint16_t) &DDRH, (uint16_t) &PORTH,PH7},{(uint16_t) &DDRG, (uint16_t) &PORTG,PG3},{(uint16_t) &DDRG, (uint16_t) &PORTG,PG4},
    {0,0,0}, // Reset
    {0,0,0}, // VCC
    {0,0,0}, // GND
    {0,0,0}, // XTAL1
    {0,0,0}, // XTAL2  
    {(uint16_t) &DDRL, (uint16_t) &PORTL,PL0},{(uint16_t) &DDRL, (uint16_t) &PORTL,PL1},{(uint16_t) &DDRL, (uint16_t) &PORTL,PL2},{(uint16_t) &DDRL, (uint16_t) &PORTL,PL3},{(uint16_t) &DDRL, (uint16_t) &PORTL,PL4},{(uint16_t) &DDRL, (uint16_t) &PORTL,PL5},{(uint16_t) &DDRL, (uint16_t) &PORTL,PL6},{(uint16_t) &DDRL, (uint16_t) &PORTL,PL7}, 
    {(uint16_t) &DDRD, (uint16_t) &PORTD,PD0},{(uint16_t) &DDRD, (uint16_t) &PORTD,PD1},{(uint16_t) &DDRD, (uint16_t) &PORTD,PD2},{(uint16_t) &DDRD, (uint16_t) &PORTD,PD3},{(uint16_t) &DDRD, (uint16_t) &PORTD,PD4},{(uint16_t) &DDRD, (uint16_t) &PORTD,PD5},{(uint16_t) &DDRD, (uint16_t) &PORTD,PD6},{(uint16_t) &DDRD, (uint16_t) &PORTD,PD7},
    {(uint16_t) &DDRG, (uint16_t) &PORTG,PG0},{(uint16_t) &DDRG, (uint16_t) &PORTG,PG1},
    {(uint16_t) &DDRC, (uint16_t) &PORTC,PC0},{(uint16_t) &DDRC, (uint16_t) &PORTC,PC1},{(uint16_t) &DDRC, (uint16_t) &PORTC,PC2},{(uint16_t) &DDRC, (uint16_t) &PORTC,PC3},{(uint16_t) &DDRC, (uint16_t) &PORTC,PC4},{(uint16_t) &DDRC, (uint16_t) &PORTC,PC5},{(uint16_t) &DDRC, (uint16_t) &PORTC,PC6},{(uint16_t) &DDRC, (uint16_t) &PORTC,PC7},
    {0,0,0}, // VCC
    {0,0,0}, // GND
    {(uint16_t) &DDRJ, (uint16_t) &PORTJ,PJ0},{(uint16_t) &DDRJ, (uint16_t) &PORTJ,PJ1},{(uint16_t) &DDRJ, (uint16_t) &PORTJ,PJ2},{(uint16_t) &DDRJ, (uint16_t) &PORTJ,PJ3},{(uint16_t) &DDRJ, (uint16_t) &PORTJ,PJ4},{(uint16_t) &DDRJ, (uint16_t) &PORTJ,PJ5},{(uint16_t) &DDRJ, (uint16_t) &PORTJ,PJ6},
    {(uint16_t) &DDRG, (uint16_t) &PORTG,PG2},
    {(uint16_t) &DDRA, (uint16_t) &PORTA,PA7},{(uint16_t) &DDRA, (uint16_t) &PORTA,PA6},{(uint16_t) &DDRA, (uint16_t) &PORTA,PA5},{(uint16_t) &DDRA, (uint16_t) &PORTA,PA4},{(uint16_t) &DDRA, (uint16_t) &PORTA,PA3},{(uint16_t) &DDRA, (uint16_t) &PORTA,PA2},{(uint16_t) &DDRA, (uint16_t) &PORTA,PA1},{(uint16_t) &DDRA, (uint16_t) &PORTA,PA0},
    {(uint16_t) &DDRJ, (uint16_t) &PORTJ,PJ7},
    {0,0,0}, // VCC
    {0,0,0}, // GND 
    {(uint16_t) &DDRK, (uint16_t) &PORTK,PK7},{(uint16_t) &DDRK, (uint16_t) &PORTK,PK6},{(uint16_t) &DDRK, (uint16_t) &PORTK,PK5},{(uint16_t) &DDRK, (uint16_t) &PORTK,PK4},{(uint16_t) &DDRK, (uint16_t) &PORTK,PK3},{(uint16_t) &DDRK, (uint16_t) &PORTK,PK2},{(uint16_t) &DDRK, (uint16_t) &PORTK,PK1},{(uint16_t) &DDRK, (uint16_t) &PORTK,PK0},
    {(uint16_t) &DDRF, (uint16_t) &PORTF,PF7},{(uint16_t) &DDRF, (uint16_t) &PORTF,PF6},{(uint16_t) &DDRF, (uint16_t) &PORTF,PF5},{(uint16_t) &DDRF, (uint16_t) &PORTF,PF4},{(uint16_t) &DDRF, (uint16_t) &PORTF,PF3},{(uint16_t) &DDRF, (uint16_t) &PORTF,PF2},{(uint16_t) &DDRF, (uint16_t) &PORTF,PF1},{(uint16_t) &DDRF, (uint16_t) &PORTF,PF0},
    {0,0,0}, // AREF
    {0,0,0}, // GND
    {0,0,0}, // AVCC
};

static unsigned digitalPinMap[] = { // Maps digital pin value on board to Pin Map Table index
    2, 3, 6, 7, 1, 5, 15, 16, 17, 18, 23,
    24, 25, 26, 64, 63, 13, 12, 46, 45, 44,
    43, 78, 77, 76, 75, 74, 73, 72, 71, 60,
    59, 58, 57, 56, 55, 54, 53, 50, 70, 52,
    51, 42, 41, 40, 39, 38, 37, 36, 35, 22,
    21, 20, 19
};

// Converts digital pin value (0-53) to index for the Pin Map Table
unsigned digitalPinToHardware( unsigned pin ) {
    return digitalPinMap[ pin ];
}

// Converts analog pin value (A0-A15) to index for the Pin Map Table
unsigned analogPinToHardware( unsigned pin ) {
    return 97-pin;
}

static inline void digitalPinModeLowlevel( volatile uint16_t *reg, volatile uint16_t *port, uint8_t pin, unsigned mode ) {
	if( mode == INPUT ) {
		*reg  &= ~(1 << pin);
		*port &= ~(1 << pin);	
	} else if( mode == INPUT_PULLUP ) {
		*reg &= ~(1 << pin);
		*port |= (1 << pin);
	} else {
		*reg |= (1 << pin);
	}
}

void digitalPinMode( unsigned pin, enum PinMode mode ) {
    PinMap mappedPin = pinMapTable[ digitalPinToHardware( pin ) ];
	digitalPinModeLowlevel( (uint16_t *)(mappedPin.reg), (uint16_t *)(mappedPin.port), mappedPin.bitOffset, mode );
}

void analogPinMode( unsigned pin, enum PinMode mode ) {

};

void pinMode( unsigned pin, enum PinType type, enum PinMode mode ) {

}

static inline void digitalWriteLowlevel( volatile uint16_t *port, uint8_t pin, unsigned value ) {
	if( value ) {
		*port |= (1 << pin);
	} else {
		*port &= ~(1 << pin);
	}
}

void digitalWrite( unsigned pin, unsigned value ) {
	unsigned index = digitalPinToHardware( pin );
    PinMap mappedPin = pinMapTable[ index ];
	digitalWriteLowlevel( (uint16_t *)mappedPin.port, mappedPin.bitOffset, value );
}

unsigned digitalRead( unsigned pin ) {
    PinMap mappedPin = pinMapTable[ digitalPinToHardware( pin ) ];
    return (mappedPin.port >> mappedPin.bitOffset) & 0x1;
}

void analogWrite( unsigned pin, unsigned value ) {

}

unsigned analogRead( unsigned pin ) {
	return 0;
}
