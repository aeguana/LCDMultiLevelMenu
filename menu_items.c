#include <stdbool.h>
#include <stdio.h>

#include "lcd.h"
#include "menu.h"
#include "menu_items.h"


/*
** Callback functions menu
*/

/*************************************************************************
 Universal callback
*************************************************************************/
void menuUniversalCallbackRender(uint8_t which){
	char buf[60];
	snprintf(buf, 60, "Clicked callback.\nLine %d clicked.", which + 1);
	lcd_clrscr();
	lcd_puts(buf);
}

bool menuUniversalCallback(MENU_BUTTON *button, uint8_t column){
	switch(button->role){
		case MENU_UP:
			break;
		case MENU_DOWN:
			break;
		case MENU_CONFIRM:
			break;
		case MENU_CANCEL:
			return true;
	}

	menuUniversalCallbackRender(column);
	return false;
}

/*************************************************************************
 Submenu custom callback
*************************************************************************/
static int submenu_number = 0;
void submenu3ActionCallbackRender(uint8_t which){
	char buf[80];
	snprintf(buf, 80, "Press UP or DOWN to\nchange number:\n%d", submenu_number);
	lcd_clrscr();
	lcd_puts(buf);
}

bool submenu3ActionCallback(MENU_BUTTON *button, uint8_t column){
	switch(button->role){
		case MENU_UP:
			++submenu_number;
			break;
		case MENU_DOWN:
			--submenu_number;
			break;
		case MENU_CONFIRM:
			return true;
		case MENU_CANCEL:
			return true;
	}
	
	char buf[80];
	snprintf(buf, 80, "Press UP or DOWN to\nchange number:\n%d", submenu_number);
	lcd_clrscr();
	lcd_puts(buf);

	return false;
}

/*************************************************************************
  Submenu custom callback
*************************************************************************/
void submenu3PrintCallbackRender(uint8_t which){
	char buf[80];
	snprintf(buf, 80, "Custom message\nfor sub menu.");
	lcd_clrscr();
	lcd_puts(buf);
}

bool submenu3PrintCallback(MENU_BUTTON *button, uint8_t column){
	switch(button->role){
		case MENU_UP:
			break;
		case MENU_DOWN:
			break;
		case MENU_CONFIRM:
			return true;
		case MENU_CANCEL:
			return true;
	}

	submenu3PrintCallbackRender(column);
	return false;
}

/*************************************************************************
 Menu 1, sub sub submenu definition
*************************************************************************/
# define SUBMENU_3_ITEMS  3
static MENU_ITEM submenu_3[SUBMENU_3_ITEMS] = {
	{"Action",   submenu3ActionCallbackRender,     submenu3ActionCallback,       0,                         NULL},
	{"Print",    submenu3PrintCallbackRender,      submenu3PrintCallback,        0,                         NULL},
	{"Empty 1",  NULL,                             NULL,                         0,                         NULL}
};

/*************************************************************************
 Menu 1, sub submenu definition
*************************************************************************/
# define SUBMENU_2_ITEMS  4
static MENU_ITEM submenu_2[SUBMENU_2_ITEMS] = {
	{"Submenu", NULL,                        NULL,                  SUBMENU_3_ITEMS, submenu_3},
	{"Empty 1", NULL,                        NULL,                  0,               NULL},
	{"Action",  menuUniversalCallbackRender, menuUniversalCallback, 0,               NULL},
	{"Empty 2", NULL,                        NULL,                  0,               NULL}
};

/*************************************************************************
 Menu 1, submenu definition
*************************************************************************/
# define MENU_1_SUBMENU_ITEMS  2
static MENU_ITEM menu_1_submenu[MENU_1_SUBMENU_ITEMS] = {
	{"Action",  menuUniversalCallbackRender,  menuUniversalCallback,  0,                       NULL     },
	{"Submenu", NULL,                         NULL,                   SUBMENU_2_ITEMS,         submenu_2}
};

/*
** HOME menu items definition
*/
#define MENU_HOME_ITEMS  10
static MENU_ITEM home_items[MENU_HOME_ITEMS] = {
	{"Menu 1",    NULL,                           NULL,                     MENU_1_SUBMENU_ITEMS,     menu_1_submenu},
	{"Menu 2",    menuUniversalCallbackRender,    menuUniversalCallback,    0,                        NULL          },
	{"Menu 3",    NULL,                           NULL,                     0,                        NULL          },
	{"Menu 4",    NULL,                           NULL,                     0,                        NULL          },
	{"Menu 5",    NULL,                           NULL,                     0,                        NULL          },
	{"Menu 6",    NULL,                           NULL,                     0,                        NULL          },
	{"Menu 7",    NULL,                           NULL,                     0,                        NULL          },
	{"Menu 8",    NULL,                           NULL,                     0,                        NULL          },
	{"Menu 9",    NULL,                           NULL,                     0,                        NULL          },
	{"Menu 10",   NULL,                           NULL,                     0,                        NULL          }
};

/*
** Public methods to access home view and home view size
*/

MENU_ITEM *menuItemsGetHomeMenu(){
	return home_items;	
}

uint8_t menuItemsGetHomeMenuSize(){
	return MENU_HOME_ITEMS;
}
