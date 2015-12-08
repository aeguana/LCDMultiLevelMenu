#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <avr/io.h>


/*
* General menu buttons
*/
#define MENU_PORTSIO_DDR					DDRA				// used for buttons in PCB
#define MENU_PORTSIO_PIN					PINA
#define MENU_PORTSIO_PORT	     		    PORTA
#define MENU_PORTSIO_BTN_UP	        	    PORTA4				// button UP
#define MENU_PORTSIO_BTN_DOWN		        PORTA3				// button DOWN
#define MENU_PORTSIO_BTN_CONFIRM	        PORTA2      		// button CONFIRM
#define MENU_PORTSIO_BTN_CANCEL	            PORTA1      		// button CANCEL

void menuPortsioInit();

#endif /* SETTINGS_H_ */
