#include "ui.h"
#include "lcd.h"
#include "oled.h"
#include "SDCard.h"
#include "SetColor.h"

void resistorExponentRefactor( String *magnitude, String *exponent );

#define mainMenuItemCount 4
const char *mainMenuItems[mainMenuItemCount] = {
  (char*)"Add Inventory",
  (char*)"Manage Stock",
  (char*)"View Stock",
  (char*)"Strobe Stock LED",
};

UIState uiState = {0};

void state_init() {
  resetAllStates();
}

void resetAllStates() {
  uiState.currentMenu = RESISTOR_SELECT;
  uiState.stateStatus = REFRESH;
  resetMainMenuState();
  resetResistorSelectState();
  resetResistorCheckout();
  resetResistorIndicator();
}

void resetMainMenuState() {
  ledOff();
  uiState.menu.selectedItem = 0;
  uiState.menu.menuItems = mainMenuItems;
  uiState.menu.menuItemsCount = mainMenuItemCount;
}

void resetResistorSelectState() {
  ledOff();
  uiState.resistorSelect.nextInputExponent = 0;
  uiState.resistorSelect.mode = NUMERIC;
  uiState.resistorSelect.magnitude.remove(0, uiState.resistorSelect.magnitude.length());
  uiState.resistorSelect.exponent.remove(0, uiState.resistorSelect.exponent.length());
}

void drawState();

void state_btn_held( char input ) {
  Serial.println("BTN HELD");
  //if ( uiState.currentMenu == RESISTOR_SELECT ) {
    if ( (KeypadButton)input == MODE_E ) {      
      uiState.currentMenu = MAIN_MENU;
      uiState.stateStatus = REFRESH;
      resetResistorSelectState();
    }
  //}
  redraw_state();
}


void update_state( char input ) {
  if ( uiState.currentMenu == MAIN_MENU ) {
    switch ( (KeypadButton)input ) {
      case UP:
        if ( uiState.menu.selectedItem > 0 ) {
          uiState.menu.selectedItem--;
          uiState.stateStatus = DIRTY;
        }
        break;
      case DOWN:
        if ( uiState.menu.selectedItem < mainMenuItemCount - 1 ) {
          uiState.menu.selectedItem++;
          uiState.stateStatus = DIRTY;
        }
        break;
      case BACK:
        uiState.currentMenu = RESISTOR_SELECT;
        uiState.stateStatus = REFRESH;
        resetMainMenuState();
        break;
      default:
        break;
    }
  } else if ( uiState.currentMenu == RESISTOR_SELECT ) {
    switch( (KeypadButton)input ) {
      case UP:
      case DOWN:
        break; 	
      case BACK:
      {
        uiState.resistorSelect.nextInputExponent = 0;
        if ( uiState.resistorSelect.exponent.charAt(0) != 0 ) {
          uiState.resistorSelect.exponent.remove(0);
          uiState.stateStatus = DIRTY;
        } else {
          if ( uiState.resistorSelect.magnitude.length() > 0 ) {
            if( uiState.resistorSelect.magnitude.charAt(uiState.resistorSelect.magnitude.length() - 1) == '.' ) {
              uiState.resistorSelect.decimal = 0;
            }
            uiState.resistorSelect.magnitude.remove(uiState.resistorSelect.magnitude.length() - 1);
            uiState.stateStatus = DIRTY;
          }
        }
      } break;
      case OK:
      {
        uiState.currentMenu = RESISTOR_CHECKOUT;
        checkInventoryForResistor();
        uiState.stateStatus = REFRESH;
      } break;
      case MODE_E:
      {
        if ( uiState.resistorSelect.magnitude.length() == 0 ) {
          uiState.resistorSelect.mode = uiState.resistorSelect.mode ? NUMERIC : COLOR;
          uiState.stateStatus = DIRTY;
        } else {
          if( uiState.resistorSelect.mode == COLOR ) break;
          uiState.resistorSelect.nextInputExponent = 1;
        }
      } break;
      case DOT:
      {
        if( uiState.resistorSelect.mode == COLOR ) break;
        if( !uiState.resistorSelect.nextInputExponent && !uiState.resistorSelect.decimal ) {		      
            uiState.resistorSelect.magnitude += '.';
            uiState.resistorSelect.decimal = 1;
            uiState.stateStatus = DIRTY;
        }
      } break;
      default:
      {
        if ( uiState.resistorSelect.nextInputExponent ) {
          if( input-48 >= 7 ) break;
          uiState.resistorSelect.exponent = input;
          uiState.resistorSelect.nextInputExponent = 0;
          uiState.stateStatus = DIRTY;
        } else {
          if ( uiState.resistorSelect.magnitude.length() < (3 + uiState.resistorSelect.decimal) ) {
            uiState.resistorSelect.magnitude += input;
            uiState.stateStatus = DIRTY;
          } else if( uiState.resistorSelect.mode == COLOR ) {
            uiState.resistorSelect.exponent = input;
            uiState.stateStatus = DIRTY;
          }
        }
      } break;
    }
  } else if( uiState.currentMenu == RESISTOR_INVENTORY_NOT_FOUND ) {
    switch( (KeypadButton)input ) {
      case OK:
      {
        uiState.currentMenu = RESISTOR_CHECKOUT;
        uiState.stateStatus = REFRESH;
        
        uiState.resistorSelect.magnitude = uiState.resistorSelect.closest.magnitude;
        uiState.resistorSelect.exponent = uiState.resistorSelect.closest.exponent;
        uiState.resistorIndicator.moduleNum = uiState.resistorSelect.closest.moduleNum;
        uiState.resistorIndicator.drawerNum = uiState.resistorSelect.closest.drawerNum; 
      } break;
      case BACK:
      {
        uiState.currentMenu = RESISTOR_SELECT;
        uiState.stateStatus = REFRESH;
      } break;
      default:
      {
        
      } break;
    }
  } else if( uiState.currentMenu == RESISTOR_CHECKOUT ) {
    switch( (KeypadButton)input ) {
      case UP:
      case DOWN:
      case DOT:
      case MODE_E:
        break;
      case OK:
      {
	      uiState.currentMenu = RESISTOR_INDICATOR;
		    uiState.stateStatus = REFRESH;
      } break;
      case BACK:
      {
        if( uiState.resistorCheckout.numOfResistors.charAt(0) == '0' ) {
          uiState.currentMenu = RESISTOR_SELECT;
          uiState.stateStatus = REFRESH;
          break;
        }
        uiState.resistorCheckout.numOfResistors.remove(uiState.resistorCheckout.numOfResistors.length()-1);
        uiState.stateStatus = DIRTY;
        if( uiState.resistorCheckout.numOfResistors.length() == 0 ) {
          uiState.resistorCheckout.numOfResistors += "0";
        }
      } break;
      default:
      {
        if( uiState.resistorCheckout.numOfResistors.length() >= 2 ) {
          return;
        }
        if( uiState.resistorCheckout.numOfResistors.charAt(0) == '0' ) {
          uiState.resistorCheckout.numOfResistors.remove(0);
        }
        uiState.resistorCheckout.numOfResistors += input;
        uiState.stateStatus = DIRTY;
      } break;
    }
  } else if( uiState.currentMenu == RESISTOR_INDICATOR ) {
    resetAllStates();
    uiState.stateStatus = REFRESH;
  }
}

void redraw_state() {
  if ( uiState.stateStatus == CURRENT ) return;

  switch ( uiState.currentMenu ) {
    case RESISTOR_SELECT:
      drawResistorSelection();
      break;
    case MAIN_MENU:
      drawMainMenu();
      break;
    case RESISTOR_CHECKOUT:
      drawResistorCheckout();
      break;
    case RESISTOR_INDICATOR:
      drawResistorIndicator();
      break;
    case RESISTOR_INVENTORY_NOT_FOUND:
      drawResistorNotFound();
      break;
    default:
      break;
  }
  uiState.stateStatus = CURRENT;
}

void drawMainMenu() {
  oled->fillScreen( OLED_BLACK );
  oled->setCursor( 0, 0 );
  oled->setTextColor( OLED_CYAN );
  oled->print( "Main Menu\n\n" );
  lcd->clearDisplay();

  for ( unsigned i = 0; i < uiState.menu.menuItemsCount; ++i ) {
    if ( i == uiState.menu.selectedItem ) {
      oled->setTextColor( OLED_YELLOW );
      oled->print( uiState.menu.menuItems[i] );
      oled->print( " <" );
    } else {
      oled->setTextColor( OLED_WHITE );
      oled->print( uiState.menu.menuItems[i] );
    }

    oled->print( "\n" );
  }
}

#define BLACK  0x0000
#define BROWN  0x6180
#define RED    0xE861 //0xF800
#define ORANGE 0xEC81 //0xFAC0
#define YELLOW 0xFFE0
#define GREEN  0x2567 //0x07E0
#define BLUE   0x2235 //0x001F
#define VIOLET 0xD81F //0x7191
#define GREY   0x7BCF //0xAD55
#define WHITE  0xFFFF

#define RESISTOR_START_X 30
#define RESISTOR_START_Y 39
#define RESISTOR_WIDTH   10
#define RESISTOR_HEIGHT  45

unsigned resistorColors[] = {BLACK, BROWN, RED, ORANGE, YELLOW, GREEN, BLUE, VIOLET, GREY, WHITE};

void drawResistor( String magnitude, String exponent );
void drawResistorSelection() {
  drawResistor( uiState.resistorSelect.magnitude, uiState.resistorSelect.exponent );
  oled->setCursor( 4, 0 );
  oled->setTextColor( OLED_YELLOW );
  oled->print( "RESISTOR SELECT MODE " );
  oled->setTextColor( OLED_RED );
  oled->setCursor( 64 - (uiState.resistorSelect.mode ? 3 : 4) * 5, 12 );
  oled->print( uiState.resistorSelect.mode ? "COLOR" : "NUMERIC" );
  lcd->displayNumber( uiState.resistorSelect.magnitude, uiState.resistorSelect.exponent );
}

void drawResistor( String magnitude, String exponent ) {
  oled->fillScreen(OLED_BLACK);

  static int resistor_color = 0xEE6D;
  oled->fillCircle( 25, 39 + 22, 26, resistor_color );
  oled->fillCircle( 127 - 25, 39 + 22, 26, resistor_color );
  oled->fillRect( 20, 39 - 1, 88, 47, resistor_color );

  int i, offset = 0;

  for ( i = 0; i < magnitude.length(); ++i ) {
    if( magnitude.charAt(i) == '.' ) continue;
    if( isDigit(magnitude.charAt(i)) ) {
      oled->fillRect( RESISTOR_START_X + (offset * 20), RESISTOR_START_Y, RESISTOR_WIDTH, RESISTOR_HEIGHT, resistorColors[magnitude.charAt(offset) - 48] );
      oled->drawRect( RESISTOR_START_X + (offset * 20), RESISTOR_START_Y, RESISTOR_WIDTH, RESISTOR_HEIGHT, BLACK );
      offset++;
    }
  }

  if ( isDigit(exponent.charAt(0)) ) {
    oled->fillRect( RESISTOR_START_X + (3 * 20), RESISTOR_START_Y, RESISTOR_WIDTH, RESISTOR_HEIGHT, resistorColors[exponent.charAt(0) - 48] );
    oled->drawRect( RESISTOR_START_X + (3 * 20), RESISTOR_START_Y, RESISTOR_WIDTH, RESISTOR_HEIGHT, BLACK );
  }

}

void resetResistorCheckout() {
  uiState.resistorCheckout.numOfResistors = String(0);
}

void resetResistorIndicator() {
  uiState.resistorIndicator.moduleNum = -1;
  uiState.resistorIndicator.drawerNum = -1;
}

void drawResistorCheckout() {
  uiState.resistorSelect.decimal = 0;
  drawResistor( uiState.resistorSelect.magnitude, uiState.resistorSelect.exponent );
  oled->setCursor( 0, 0 );
  oled->setTextColor( OLED_RED );
  oled->print( "Enter Amount: " );

  String magnitude = uiState.resistorSelect.magnitude;
  String exponent = uiState.resistorSelect.exponent;
  resistorExponentRefactor( &magnitude, &exponent );
  lcd->displayResistorCheckout( magnitude, exponent, uiState.resistorCheckout.numOfResistors );

}

void drawResistorIndicator() {
  drawResistor( uiState.resistorSelect.magnitude, uiState.resistorSelect.exponent );
  ledOn( uiState.resistorIndicator.moduleNum, uiState.resistorIndicator.drawerNum );
}

unsigned long resistorStringToInt( String magnitude, String exponent ) {
  String resistorString;
  unsigned decimalOffset = 0;
  
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

  return (unsigned long)resistorString.toInt();
}

void resistorExponentRefactor( String *magnitude, String *exponent ) {
  String newMag, newExp;
  int exponentValue = exponent->toInt();
  for( int i = 0; i < magnitude->length(); ++i ) {
    if( isdigit(magnitude->charAt(i) ) ) {
      newMag += magnitude->charAt(i);
    }
  }
  
  if( exponentValue < 3 ) {
    for( int i = 0; i < exponentValue; ++i ) {
      newMag += '0';
    }
    newExp = '0';
  } else if( exponentValue < 6 ) {
    for( int i = 0; i < exponentValue-3; ++i ) {
      newMag += '0';
    }
    newExp = '3';
  } else {
    for( int i = 0; i < exponentValue-6; ++i ) {
      newMag += '0';
    }
    
    newExp = '6';
  }
  (*magnitude) = newMag;
  (*exponent) = newExp;
}

void checkInventoryForResistor() {
  if( uiState.resistorSelect.mode == COLOR ) {
    int magnitudeLen = uiState.resistorSelect.magnitude.length();
    uiState.resistorSelect.exponent = uiState.resistorSelect.magnitude.charAt(magnitudeLen-1);
    uiState.resistorSelect.magnitude.remove(magnitudeLen-1, 1);
  }

  unsigned long inputResistance = resistorStringToInt( uiState.resistorSelect.magnitude, uiState.resistorSelect.exponent );
  
  InventoryInfo closestValue = sd_find_closest_resistor( uiState.resistorSelect.magnitude, uiState.resistorSelect.exponent );

  unsigned long closestResistance = resistorStringToInt( closestValue.magnitude, closestValue.exponent );
  
  if( inputResistance != closestResistance ) {
    uiState.resistorSelect.closest = closestValue;
    uiState.currentMenu = RESISTOR_INVENTORY_NOT_FOUND;
  } else {
    uiState.resistorSelect.magnitude = closestValue.magnitude;
    uiState.resistorSelect.exponent = closestValue.exponent;
    uiState.resistorIndicator.moduleNum = closestValue.moduleNum;
    uiState.resistorIndicator.drawerNum = closestValue.drawerNum; 
  }
}

void drawResistorNotFound() {
  lcd->clearDisplay();
  String msg;
  /*msg += uiState.resistorSelect.magnitude;
  msg += "e";
  msg += uiState.resistorSelect.exponent;*/
  String magnitude = uiState.resistorSelect.closest.magnitude;
  String exponent = uiState.resistorSelect.closest.exponent;
  resistorExponentRefactor( &magnitude, &exponent );
  
  lcd->displayNotInStock( magnitude, exponent );

  oled->fillScreen(OLED_BLACK);
  oled->setCursor( 0, 0 );
  oled->setTextColor( OLED_RED );
  oled->print( "Selected resistor is NOT in stock\n\n" );
  oled->setTextColor( OLED_WHITE );
  oled->print( "Would you like to\npurchase closest\navailable?\n\n" );
  oled->setTextColor( OLED_YELLOW );
  oled->print( "Yes(Ok) / No(Back)" );
}
