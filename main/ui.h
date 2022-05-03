#ifndef _UI_H
#define _UI_H

#include <Arduino.h>
#include "keypad.h"
#include "SDCard.h"
/*
	Keypad Layout
	'1', '2', '3', 'u'
	'4', '5', '6', 'd'
	'7', '8', '9', 'b'
	'.', '0', 'E', 'e'
*/


enum MenuState {
  RESISTOR_SELECT,
  RESISTOR_CHECKOUT,
  RESISTOR_INVENTORY_NOT_FOUND,
  RESISTOR_INDICATOR,
  MAIN_MENU,
  ADD_INVENTORY,
  MANAGE_STOCK,
  VIEW_STOCK,
  ERROR,
};

enum StateStatus {
  DIRTY, // State has small incremental change
  REFRESH, // State has completely changed, new screen
  CURRENT, // State hasn't changed, no redraw required
};

typedef struct {
  char *button_name;
  void (*fn_ptr)(void*);
} UIButton;

enum ResistorSelectMode {
  NUMERIC,
  COLOR,
};

typedef struct {
  String magnitude, exponent;
  int nextInputExponent;
  enum ResistorSelectMode mode;
  int decimal;
  InventoryInfo closest;
} ResistorSelection;

typedef struct {
  String numOfResistors;
} ResistorCheckout;

typedef struct {
  int moduleNum;
  int drawerNum;
} ResistorIndicator;

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

typedef struct {
  enum StateStatus stateStatus;
  enum  MenuState currentMenu;
  ResistorSelection resistorSelect;
  ResistorCheckout resistorCheckout;
  ResistorIndicator resistorIndicator;
  MainMenu menu;
  AddInventory addInventory;
  ManageStock manageStock;
  ViewStock viewStock;
} UIState;

extern UIState uiState;

void state_init();
void update_state( char input );
void redraw_state();
void state_btn_held( char input );

void resetAllStates();
void resetMainMenuState();
void resetResistorSelectState();
void resetResistorCheckout();
void resetResistorIndicator();

void checkInventoryForResistor();

void drawMainMenu();
void drawAddInventory();
void drawManageStock();
void drawViewStock();
void drawResistorSelection();
void drawResistorCheckout();
void drawResistorIndicator();
void drawResistorNotFound();

#endif /* _UI_H */
