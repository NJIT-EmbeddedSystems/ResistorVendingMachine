#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include "util.h"
#include <avr/pgmspace.h>


int main( void ) {
    digitalPinMode( 12, OUTPUT );

    while( 1 ) {
        digitalWrite( 12, HIGH );
		_delay_ms( 1000 );
        digitalWrite( 12, LOW );
        _delay_ms( 1000 );
    }

    return 0;
}
