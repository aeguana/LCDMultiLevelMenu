#include "menu_portsio.h"


static void setupPortsForControlButtons(){
	// Set buttons as input
	MENU_PORTSIO_DDR &= ~(_BV(MENU_PORTSIO_BTN_UP)     |
					     _BV(MENU_PORTSIO_BTN_DOWN)    |
					     _BV(MENU_PORTSIO_BTN_CONFIRM) |
					     _BV(MENU_PORTSIO_BTN_CANCEL));

	// with active low
	MENU_PORTSIO_PORT |= _BV(MENU_PORTSIO_BTN_UP)      |
					     _BV(MENU_PORTSIO_BTN_DOWN)    |
					     _BV(MENU_PORTSIO_BTN_CONFIRM) |
					     _BV(MENU_PORTSIO_BTN_CANCEL);
}

void menuPortsioInit(){
	setupPortsForControlButtons();
}