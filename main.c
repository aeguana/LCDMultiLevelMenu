#include <avr/io.h>

#include "menu.h"
#include "lcd.h"


int main(void){
	lcd_init(LCD_DISP_ON);
	menuInit();

    while(1){
        menuPollButtons();
    }
}