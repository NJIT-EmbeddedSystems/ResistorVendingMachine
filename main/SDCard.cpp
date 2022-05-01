#include <SPI.h>
#include <SD.h>

#include "SDCard.h"
#include "pindef.h"

File resistorInventoryFile;
unsigned long startOfLine;

void sdCard_init() {
    //initalize SD card
    //pinMode( SD_CS_PIN, OUTPUT );
    if( !SD.begin(SD_CS_PIN) ) {
      Serial.println( "Card failed, or not present" );
      return;
    }
    
    startOfLine = 0;
    
    resistorInventoryFile = SD.open("data.txt");
    if( resistorInventoryFile ) {
      Serial.println( "File Found" );
        //SD.close("data.txt");
    } else {
      Serial.println( "File Not Found" );
    }
}

void moveToCommaElement( int element ) {
  resistorInventoryFile.seek( startOfLine );
  while( resistorInventoryFile.peek() == '#' ) {
    sd_next_line();
  }

  for(int i = 0; i < element; ++i ) {
    while( resistorInventoryFile.available() && resistorInventoryFile.read() != ',' );
  }
}

unsigned long sd_read_resistor_value( void ) {
  String buffer;
  int exponent_decimal_offset = 0;

  int foundDot = 0;
  
  moveToCommaElement( 0 ); 

  char c;
  while( resistorInventoryFile.available() ) {
    c = resistorInventoryFile.read();
    if( isdigit(c) ) {
	  buffer += c;
 	  if( foundDot ) {
        exponent_decimal_offset++;
      }
      continue;
    }

    if( c == '.' ) {
      foundDot = 1;
    }

    if( c == ',' || tolower(c) == 'e' ) {
      break;
    }
  }

  if( tolower(c) == 'e' ) {
	c = resistorInventoryFile.read();
    if( isdigit( c ) ) {
      for( int i = 0; i < ((c-48)-exponent_decimal_offset); ++i ) {
        buffer += '0';
      }
    }
  }
  
  return buffer.toInt();
}

void sd_read_resistor_color_code( String *magnitude, String *exponent ) {
  moveToCommaElement( 1 );
 
  char c;
  while( resistorInventoryFile.available() && (c = resistorInventoryFile.read()) != ',' ) {
  	if( resistorInventoryFile.peek() == ',' ) {
  		(*exponent) = c;
  	} else {
  		(*magnitude) += c;
  	}
  }  
  (*magnitude).trim();
  (*exponent).trim();
}

unsigned sd_read_inventory_count( void ) {
  moveToCommaElement( 2 );

  String count;
  char c;
  while( resistorInventoryFile.available() && (c = resistorInventoryFile.read()) != ',' ) {
    count += c; 
  }

  return count.toInt();
}

unsigned sd_read_module_num( void ) {
  moveToCommaElement(3);

  String num;
  char c;
  while( resistorInventoryFile.available() && (c = resistorInventoryFile.read()) != ',' ) {
    num += c; 
  }
 
  return num.toInt();
}

unsigned sd_read_drawer_num( void ) {
  moveToCommaElement(4);

  String num;
  char c;
  while( resistorInventoryFile.available() && (c = resistorInventoryFile.read()) != ',' ) {
    num += c; 
  }
  return num.toInt();
}

// Returns false if end of file
bool sd_next_line( void ) {
  resistorInventoryFile.seek( startOfLine );
  while( resistorInventoryFile.available() && resistorInventoryFile.read() != '\n' );
  startOfLine = resistorInventoryFile.position();
  return resistorInventoryFile.available();
}

bool sd_previous_line( void ) {
  
  return false;
}

void sd_goto_start( void ) {
  resistorInventoryFile.seek( 0 );
  startOfLine = 0;
}
