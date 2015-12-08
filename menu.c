#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

#include "menu_portsio.h"
#include "lcd.h"
#include "menu.h"
#include "menu_items.h"

/*
** Menu main components definition
*/

/*************************************************************************
 All possible menu states, depend how many nest actions we want
*************************************************************************/
static MENU_STATE menu_states[MENU_LEVEL_COUNT] = {
	{NULL, NULL, 0, 0, MENU_LEVEL_1},
	{NULL, NULL, 0, 0, MENU_LEVEL_2},
	{NULL, NULL, 0, 0, MENU_LEVEL_3},
	{NULL, NULL, 0, 0, MENU_LEVEL_4},
	{NULL, NULL, 0, 0, MENU_CALLBACK}
};

/*************************************************************************
 Assign menu state default value to first one (home menu)
*************************************************************************/
static MENU_STATE *current_state = &menu_states[0];

/*************************************************************************
 Map all menu action buttons to the real pins and assign functionality
*************************************************************************/
static MENU_BUTTON menu_buttons[MENU_BUTTON_ROLE_COUNT] = {
	{MENU_UP,      MENU_PORTSIO_BTN_UP,      0, false},
	{MENU_DOWN,    MENU_PORTSIO_BTN_DOWN,    0, false},
	{MENU_CONFIRM, MENU_PORTSIO_BTN_CONFIRM, 0, false},
	{MENU_CANCEL,  MENU_PORTSIO_BTN_CANCEL,  0, false}
};

/*
** Render and control functions
*/
/*************************************************************************
 Helpers for setting the state properties
*************************************************************************/
static void setState(MENU_STATE *state, MENU_STATE *new_prev_state, MENU_ITEM *new_current_item, uint8_t new_current_item_idx, uint8_t new_cursor_line){
	state->prev_state = new_prev_state;
	state->current_item = new_current_item;
	state->current_item_idx = new_current_item_idx;
	state->cursor_line = new_cursor_line;
}

static void resetState(MENU_STATE *state){
	setState(state, NULL, NULL, 0, 0);
}

/*************************************************************************
 Basic render menu function, responsible for rendering the menu
*************************************************************************/
static void renderMenu(){
	lcd_clrscr();

	MENU_ITEM *items = menuItemsGetHomeMenu();
	uint8_t max_options = menuItemsGetHomeMenuSize();
	
	if(current_state->level != MENU_LEVEL_1){
		MENU_ITEM *previous_item = menu_states[current_state->prev_state->level].current_item;
		items = previous_item->submenu;
		max_options = previous_item->submenu_length;
	}

	uint8_t first_item = current_state->current_item_idx - current_state->cursor_line;

	for(int i=0; i<max_options && i<LCD_LINES; i++){
		lcd_gotoxy(0, i);
		if(i==current_state->cursor_line) lcd_putc('>');
		else lcd_putc(' ');
		lcd_putc(' ');
		lcd_puts(items[first_item+i].title);
	}
}

/***************************************************************************************
 Function to handle basic btn clicks (Executed when menu is not in CALLBACK state)
***************************************************************************************/
static void scrollMenu(MENU_BUTTON *button){
	switch(button->role){
		case MENU_UP:
			if(current_state->current_item_idx != 0){
				if(current_state->cursor_line == 0){
					// Move screen
					current_state->current_item_idx -= 1;
				}else{
					// Move cursor
					current_state->cursor_line -= 1;
					current_state->current_item_idx -= 1;
				}
			}
			break;
		case MENU_DOWN: {
			uint8_t max_options = menuItemsGetHomeMenuSize();
			if(current_state->level != MENU_LEVEL_1){
				MENU_ITEM *previous_item = menu_states[current_state->prev_state->level].current_item;
				max_options = previous_item->submenu_length;
			}

			if(current_state->current_item_idx < max_options-1){
				if(current_state->cursor_line == LCD_LINES - 1){
					// Move screen
					current_state->current_item_idx += 1;
				}else{
					// Move cursor
					current_state->cursor_line += 1;
					current_state->current_item_idx += 1;
				}
			}
			break;
		}
		case MENU_CONFIRM:;
			MENU_ITEM *items = menuItemsGetHomeMenu();
			if(current_state->level != MENU_LEVEL_1){
				MENU_ITEM *previous_item = menu_states[current_state->prev_state->level].current_item;
				items = previous_item->submenu;
			}
				
			MENU_ITEM *item = &items[current_state->current_item_idx];
			MENU_STATE *prev_state = current_state;
			if(item->render_callback){
				prev_state->current_item = item;
				current_state = &menu_states[MENU_CALLBACK];
				setState(current_state, prev_state, prev_state->current_item, prev_state->current_item_idx, 0);
			}else if(item->submenu){
				prev_state->current_item = item;
				current_state = &menu_states[current_state->level + 1];
				setState(current_state, prev_state, NULL, 0, 0);
			}
			break;
		case MENU_CANCEL:
			if(current_state->level != MENU_LEVEL_1){
				current_state = &menu_states[current_state->prev_state->level];
			}
			break;
	}
	if(current_state->level != MENU_CALLBACK){
		renderMenu();
	}else{
		current_state->prev_state->current_item->render_callback(current_state->current_item_idx);
	}
}

/*
** Public functions accessible from .h file
*/

void menuReset(){
	current_state = &menu_states[MENU_LEVEL_1];
	resetState(current_state);
	renderMenu();
}

void menuInit(){
	menuPortsioInit();
	menuReset();
}

void menuPollButtons(){
	for(uint8_t i=0; i<MENU_BUTTON_ROLE_COUNT; i++){
		MENU_BUTTON *b = &menu_buttons[i];
		// Eliminate button debouncing
		// http://www.ganssle.com/debouncing-pt2.htm
		b->state = (b->state << 1) | !((MENU_PORTSIO_PIN) & (1<<(b->button_PIN))) | 0XE000;

		if(b->state == 0xF000){
			b->pressed = true;
			if(current_state->level != MENU_CALLBACK){
				scrollMenu(b);
			}else{
				bool exited = current_state->prev_state->current_item->btn_handle_callback(b, current_state->current_item_idx);
				if(exited){
					current_state = &menu_states[current_state->prev_state->level];
					renderMenu();
				}
			}
		}else{
			b->pressed = false;
		}
	}
}
