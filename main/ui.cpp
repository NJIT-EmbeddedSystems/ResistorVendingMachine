#include "ui.h"
#include "lcd.h"
#include "oled.h"

#define mainMenuItemCount 4
const char *mainMenuItems[mainMenuItemCount] = {
  (char*)"Add Inventory",
  (char*)"Manage Stock",
  (char*)"View Stock",
  (char*)"Strobe Stock LED",
};

UIState uiState = {0};

void state_init() {
  uiState.currentMenu = MAIN_MENU;

  resetMainMenuState();
  resetResistorSelectState();
  resetResistorCheckout();
}

void resetMainMenuState() {
  uiState.menu.selectedItem = 0;
  uiState.menu.menuItems = mainMenuItems;
  uiState.menu.menuItemsCount = mainMenuItemCount;
}

void resetResistorSelectState() {
  uiState.resistorSelect.nextInputExponent = 0;
  uiState.resistorSelect.mode = NUMERIC;
  uiState.resistorSelect.magnitude.remove(0, uiState.resistorSelect.magnitude.length());
  uiState.resistorSelect.exponent.remove(0, uiState.resistorSelect.exponent.length());
}

void drawState();

void state_btn_held( char input ) {
  Serial.print("State Btn held: ");
  Serial.println( input );
  if ( uiState.currentMenu == RESISTOR_SELECT ) {
    if ( (KeypadButton)input == MODE_E ) {      
      uiState.currentMenu = MAIN_MENU;
      uiState.stateStatus = REFRESH;
      resetResistorSelectState();
    }
  }
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
    if ( (KeypadButton)input == BACK ) {
      uiState.resistorSelect.nextInputExponent = 0;
      if ( uiState.resistorSelect.exponent.charAt(0) != 0 ) {
        uiState.resistorSelect.exponent.remove(0);
        uiState.stateStatus = DIRTY;
      } else {
        if ( uiState.resistorSelect.magnitude.length() > 0 ) {
          uiState.resistorSelect.magnitude.remove(uiState.resistorSelect.magnitude.length() - 1);
          uiState.stateStatus = DIRTY;
        }
      }
    } else {
      if ( uiState.resistorSelect.nextInputExponent ) {
        if ( isDigit(input) ) {
          uiState.resistorSelect.exponent = input;
          uiState.resistorSelect.nextInputExponent = 0;
          uiState.stateStatus = DIRTY;
        }
      } else {
        if ( (KeypadButton)input == MODE_E ) {
          if ( uiState.resistorSelect.magnitude.length() == 0 ) {
            uiState.resistorSelect.mode = uiState.resistorSelect.mode ? NUMERIC : COLOR;
            //Serial.println( uiState.resistorSelect.mode ? "COLOR" : "NUMERIC" );
            uiState.stateStatus = DIRTY;
          } else {
            uiState.resistorSelect.nextInputExponent = 1;
          }
        } else if( (KeypadButton)input == OK ) {
          uiState.currentMenu = RESISTOR_CHECKOUT;
          uiState.stateStatus = REFRESH;
        } else if ( isDigit(input) && uiState.resistorSelect.magnitude.length() < 3 ) {
          uiState.resistorSelect.magnitude += input;
          uiState.stateStatus = DIRTY;
        }
      }
    }
  } else if( uiState.currentMenu == RESISTOR_CHECKOUT ) {
    if( isDigit(input) ) {
      if( uiState.resistorCheckout.numOfResistors.length() >= 2 ) {
        return;
      }
      if( uiState.resistorCheckout.numOfResistors.charAt(0) == '0' ) {
        uiState.resistorCheckout.numOfResistors.remove(0);
      }
      uiState.resistorCheckout.numOfResistors += input;
      uiState.stateStatus = DIRTY;
    } else if( (KeypadButton)input == BACK ) {
      uiState.resistorCheckout.numOfResistors.remove(uiState.resistorCheckout.numOfResistors.length()-1);
      if( uiState.resistorCheckout.numOfResistors.length() == 0 ) {
        uiState.resistorCheckout.numOfResistors += "0";
      }
      uiState.stateStatus = DIRTY;
    }
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
  lcd->displayNumber( uiState.resistorSelect.magnitude, uiState.resistorSelect.exponent );
}

void drawResistor( String magnitude, String exponent ) {
  oled->fillScreen(OLED_BLACK);

  oled->setCursor( 4, 0 );
  oled->setTextColor( OLED_YELLOW );
  oled->print( "RESISTOR SELECT MODE " );
  oled->setTextColor( OLED_RED );
  oled->setCursor( 64 - (uiState.resistorSelect.mode ? 3 : 4) * 5, 12 );
  oled->print( uiState.resistorSelect.mode ? "COLOR" : "NUMERIC" );

  static int resistor_color = 0xEE6D;
  oled->fillCircle( 25, 39 + 22, 26, resistor_color );
  oled->fillCircle( 127 - 25, 39 + 22, 26, resistor_color );
  oled->fillRect( 20, 39 - 1, 88, 47, resistor_color );

  int i;

  for ( i = 0; i < magnitude.length() && i < 3; ++i ) {
    if ( isDigit(magnitude.charAt(i)) ) {
      oled->fillRect( RESISTOR_START_X + (i * 20), RESISTOR_START_Y, RESISTOR_WIDTH, RESISTOR_HEIGHT, resistorColors[magnitude.charAt(i) - 48] );
    }
    oled->drawRect( RESISTOR_START_X + (i * 20), RESISTOR_START_Y, RESISTOR_WIDTH, RESISTOR_HEIGHT, BLACK );
  }

  if ( isDigit(exponent.charAt(0)) ) {
    oled->fillRect( RESISTOR_START_X + (3 * 20), RESISTOR_START_Y, RESISTOR_WIDTH, RESISTOR_HEIGHT, resistorColors[exponent.charAt(0) - 48] );
    oled->drawRect( RESISTOR_START_X + (3 * 20), RESISTOR_START_Y, RESISTOR_WIDTH, RESISTOR_HEIGHT, BLACK );
  }

}

void resetResistorCheckout() {
  uiState.resistorCheckout.numOfResistors = String(0);
}

void drawResistorCheckout() {
  oled->fillScreen(OLED_BLACK);
  oled->setCursor( 0, 0 );
  oled->setTextColor( OLED_RED );
  oled->print( "Enter Amount: " );

  static int resistor_color = 0xEE6D;
  oled->fillCircle( 25, 39 + 22, 26, resistor_color );
  oled->fillCircle( 127 - 25, 39 + 22, 26, resistor_color );
  oled->fillRect( 20, 39 - 1, 88, 47, resistor_color );

  int i;
  String magnitude = uiState.resistorSelect.magnitude;
  String exponent = uiState.resistorSelect.exponent;
  for ( i = 0; i < magnitude.length() && i < 3; ++i ) {
    if ( isDigit(magnitude.charAt(i)) ) {
      oled->fillRect( RESISTOR_START_X + (i * 20), RESISTOR_START_Y, RESISTOR_WIDTH, RESISTOR_HEIGHT, resistorColors[magnitude.charAt(i) - 48] );
    }
    oled->drawRect( RESISTOR_START_X + (i * 20), RESISTOR_START_Y, RESISTOR_WIDTH, RESISTOR_HEIGHT, BLACK );
  }

  if ( isDigit(exponent.charAt(0)) ) {
    oled->fillRect( RESISTOR_START_X + (3 * 20), RESISTOR_START_Y, RESISTOR_WIDTH, RESISTOR_HEIGHT, resistorColors[exponent.charAt(0) - 48] );
    oled->drawRect( RESISTOR_START_X + (3 * 20), RESISTOR_START_Y, RESISTOR_WIDTH, RESISTOR_HEIGHT, BLACK );
  }

  lcd->displayResistorCheckout( uiState.resistorSelect.magnitude, uiState.resistorSelect.exponent, uiState.resistorCheckout.numOfResistors );

}
