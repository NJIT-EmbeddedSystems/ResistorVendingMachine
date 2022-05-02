#ifndef _SDCARD_H

#include "SD.h"

extern File resistorInventoryFile;

typedef struct {
  String magnitude, exponent;
  unsigned long numericValue;
  unsigned count;
  unsigned moduleNum, drawerNum;
} InventoryInfo;

InventoryInfo sd_find_closest_resistor( String magnitude, String exponent );

void sdCard_init();
unsigned long sd_read_resistor_value( void );
void sd_read_resistor_color_code( String *magnitude, String *exponent );
unsigned sd_read_inventory_count( void );
unsigned sd_read_module_num( void );
unsigned sd_read_drawer_num( void );
bool sd_next_line( void );
void sd_goto_start( void );

#endif
