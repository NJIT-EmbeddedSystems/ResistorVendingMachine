#include "ui.h"

#include "oled.h"

void uiInitializeState() {
	uiState = {0};
	uiState.currentMenu = NONE;

	uiState.menu = {.selectedItem=0, .menuItems=mainMenuItems, .menuItemsCount=mainMenuItemCount};

	

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
