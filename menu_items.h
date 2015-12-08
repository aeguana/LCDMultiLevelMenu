#ifndef MENU_CALLBACK_H_
#define MENU_CALLBACK_H_

/**
 * @name  Structure to represent menu item 
 * Represents each menu item. Single MENU_ITEM can contains `render_callback` && `btn_handle_callback` or
 * `submenu_length` && `submenu`.
 * If item has first set, it will be used as final, custom action for menu.
 * If item has second set, it will be used as another step from which the menu nest will proceed.
 */
typedef struct MENU_ITEM {
	char *title;
	void (* render_callback)(uint8_t which);
	bool (* btn_handle_callback)(MENU_BUTTON *button, uint8_t which);
	uint8_t submenu_length;
	struct MENU_ITEM *submenu;
} MENU_ITEM;

MENU_ITEM *menuItemsGetHomeMenu();
uint8_t menuItemsGetHomeMenuSize();

#endif /* MENU_CALLBACK_H_ */