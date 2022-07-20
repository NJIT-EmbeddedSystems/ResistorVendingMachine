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
    
    resistorInventoryFile = SD.open("data.txt", O_RDWR);
    if( resistorInventoryFile ) {
      Serial.println( "File Found" );
        //SD.close("data.txt");
    } else {
      Serial.println( "File Not Found" );
    }
    sd_goto_start();
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
  if( buffer.length() == 0 ) {
    return 0;
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
  magnitude->remove(0, magnitude->length());
  exponent->remove(0, exponent->length());
  
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

InventoryInfo sd_find_closest_resistor( String magnitude, String exponent ) {
  InventoryInfo closest;
  
  int decimalOffset = 0; 
  String resistorString;
  unsigned long resistorValue = 0;
  
  for( int i = 0; i < magnitude.length(); ++i ) {
    if( magnitude.charAt(i) == '.' ) {
      if( exponent.toInt() == 0 ) break;
      decimalOffset = magnitude.length() - (i+1);
    } else {
      resistorString += magnitude.charAt(i);
    }
  }

  for( int i = 0; i < exponent.toInt()-decimalOffset; ++i ) {
    resistorString += '0';
  }

  resistorValue = resistorString.toInt();

  sd_goto_start();
  unsigned long currentResistor = 0, closestResistor = 0;
  while( sd_read_inventory_count == 0 ) {
    sd_next_line();
  }
  
  closestResistor = sd_read_resistor_value();
  closest.numericValue = closestResistor;
  sd_read_resistor_color_code( &closest.magnitude, &closest.exponent );
  closest.count = sd_read_inventory_count();
  closest.moduleNum = sd_read_module_num();
  closest.drawerNum = sd_read_drawer_num();
  
  sd_next_line();
  while( (currentResistor = sd_read_resistor_value()) != 0 ) {
    if( sd_read_inventory_count() == 0 ) {
      sd_next_line();
      continue;
    }
    if( abs(long(resistorValue - currentResistor)) < abs(long(resistorValue - closestResistor)) ) {
      closestResistor = currentResistor;
      closest.numericValue = closestResistor;
      sd_read_resistor_color_code( &closest.magnitude, &closest.exponent );
      closest.count = sd_read_inventory_count();
      closest.moduleNum = sd_read_module_num();
      closest.drawerNum = sd_read_drawer_num();
    }
    sd_next_line();
  }
  sd_goto_start();

  return closest;
}

void sd_update_inventory( unsigned newAmount, unsigned moduleNum, unsigned drawerNum ) {
  unsigned currentModule, currentDrawer;
  unsigned long pos = 0;
  sd_goto_start();
  for(;;) {
    currentModule = sd_read_module_num();
    currentDrawer = sd_read_drawer_num();
    
    if( currentModule == moduleNum && currentDrawer == drawerNum ) break;
    
    sd_next_line();  
  }
  
  pos = resistorInventoryFile.position();
  moveToCommaElement(3);
  unsigned long bufferSize = resistorInventoryFile.size() - resistorInventoryFile.position();
 
  char *endOfFile = (char*)calloc( bufferSize+1, sizeof(char) );
  resistorInventoryFile.read( endOfFile, bufferSize );
  sd_goto_start();

  resistorInventoryFile.seek( pos );
  moveToCommaElement(2);

  while( resistorInventoryFile.peek() == " " || resistorInventoryFile.peek() == ',' ) {
    resistorInventoryFile.read();
  }

  do {
    resistorInventoryFile.write( (char)('0' + (newAmount%10)) );
    Serial.print( "Writing " );
    Serial.println( (char)('0' + (newAmount%10)) );
    newAmount /= 10;
  } while( newAmount != 0 );
  resistorInventoryFile.write( "," );
  resistorInventoryFile.write( endOfFile, bufferSize );
  resistorInventoryFile.write( EOF );
  free( endOfFile );
  resistorInventoryFile.flush();
  sd_goto_start();

  char *file = (char*)calloc( resistorInventoryFile.size()+1, sizeof(char) );
  resistorInventoryFile.read( file, resistorInventoryFile.size() );
  Serial.println( file );
  free( file );
  
  sd_goto_start();
}
