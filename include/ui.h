#ifndef _UI_H
#define _UI_H

enum Button {
	ZERO, ONE, TWO, THREE, FOUR, FIVE,
	SIX, SEVEN, EIGHT, NINE,
	UP, DOWN,
	BACK, OK,
	DOT, MODE_E,
};

enum MenuState {
	NONE, // Base state used to input value/color
	MAIN_MENU,
	ADD_INVENTORY,
	MANAGE_STOCK,
	VIEW_STOCK,
	ERROR,
};

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

void uiInitializeState();
void uiUpdateState( /*Arg: Button Pressed*/ );
void uiRedraw();

void drawBase();
void drawError();

void drawMainMenu();
void drawAddInventory();
void drawManageStock();
void drawViewStock();

#endif /* _UI_H */
