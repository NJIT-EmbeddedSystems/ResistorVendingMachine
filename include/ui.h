#ifndef _UI_H
#define _UI_H

/*
	Keypad Layout
	'1', '2', '3', 'u'
	'4', '5', '6', 'd'
	'7', '8', '9', 'b'
	'.', '0', 'E', 'e'
*/

enum KeypadButton {
	ZERO='0', ONE, TWO, THREE, FOUR, FIVE,
	SIX, SEVEN, EIGHT, NINE,
	UP='u', DOWN='d',
	BACK='b', OK='e',
	DOT='.', MODE_E='E',
};

enum MenuState {
	RESISTOR_SELECT,
	MAIN_MENU,
	ADD_INVENTORY,
	MANAGE_STOCK,
	VIEW_STOCK,
	ERROR,
};

typedef struct {
	char *button_name;
	void (*fn_ptr)(void*);
} Button;

static unsigned mainMenuItemCount = 4;
char *mainMenuItems[mainMenuItemCount] = {
	"Add Inventory",
	"Manage Stock",
	"View Stock",
	"Strobe Stock LED",
};

typedef struct {
	unsigned selectedItem;
	char **menuItems;
	unsigned menuItemsCount;
} MainMenu;

typedef struct {

} AddInventory;

typedef struct {

} ManageStock;

typedef struct {

} ViewStock;

struct UIState{
	enum  MenuState currentMenu;
	MainMenu menu;
	AddInventory addInventory;
	ManageStock manageStock;
	ViewStock viewStock;
} uiState;

void state_init();
void update_state( char input );
void redraw_state();

void drawBase();
void drawError();

void drawMainMenu();
void drawAddInventory();
void drawManageStock();
void drawViewStock();

#endif /* _UI_H */
