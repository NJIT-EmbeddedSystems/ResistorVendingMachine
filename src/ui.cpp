#include "ui.h"

#include "oled.h"

void state_init() {
	uiState = {0};
	uiState.currentMenu = MainMenu;

	uiState.menu = {.selectedItem=0, .menuItems=mainMenuItems, .menuItemsCount=mainMenuItemCount};

}

void update_state( char input ) {
	if( uiState.currentMenu == MainMenu ) {
		switch( (KeypadButton)input ) {
			case UP:
				if( uiState.menu.selectedItem > 0 ) {
					uiState.menu.selectedItem--;
				}
				break;
			case DOWN:
				if( uiState.menu.selectedItem < mainMenuItemCount ) {
					uiState.menu.selectedItem++;
				}
				break;
			default:
				break;
		}
		drawMainMenu();
	}
}

void drawMainMenu() {
	oled.fillScreen( OLED_BLACK );
	oled.setCursor( 0, 0 );
	oled.setTextColor( OLED_CYAN );
	oled.print( "Main Menu\n\n" );
	
	for( unsigned i = 0; i < uiState.menu.menuItemsCount; ++i ) {
		if( i == uiState.menu.selectedItem ) {
			oled.setTextColor( YELLOW );
			oled.print( uiState.menu.menuItems[i] );
			oled.print( " <" );
			oled.setTextColor( WHITE );
		} else {
			oled.print( uiState.menu.menuItems[i] );
		}
	
		oled.print( "\n" );
	}
}
