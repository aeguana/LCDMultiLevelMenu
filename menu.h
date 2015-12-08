#ifndef MENU_H_
#define MENU_H_

#include <stdbool.h>

/**
 * @name  Forward declaration of menu item struct
 * Declared in menu_items.h
 */
struct MENU_ITEM;

/**
 * @name  Enum declaration for buttons functionality
 * Should not be changed as the big chunk of logic is based on it.
 */
typedef enum {
	MENU_UP,
	MENU_DOWN,
	MENU_CONFIRM,
	MENU_CANCEL
} MENU_BUTTON_ROLE;
#define MENU_BUTTON_ROLE_COUNT  4 // Length of MENU_BUTTON_ROLE items

/**
 * @name  Enum declaration for menu nest levels
 * Change when menu has more/less nest levels.
 */
typedef enum {
	MENU_LEVEL_1,
	MENU_LEVEL_2,
	MENU_LEVEL_3,
	MENU_LEVEL_4,
	MENU_CALLBACK,
}MENU_LEVEL;
#define MENU_LEVEL_COUNT        5 // Length of MENU_LEVEL items

/**
 * @name  Structure to represent menu button 
 * Represents each button in the menu, when state changed to pressed, an event will be dispatched.
 */
typedef struct {
	MENU_BUTTON_ROLE role;
	uint8_t button_PIN;
	uint16_t state;
	bool pressed;
} MENU_BUTTON;

/**
 * @name  Structure to represent overall menu state 
 * Works like a state machine and indicates/caches the actual and previous state of menu.
 */
typedef struct MENU_STATE {
	struct MENU_STATE *prev_state;
	struct MENU_ITEM *current_item;
	uint8_t current_item_idx;
	uint8_t cursor_line;
	MENU_LEVEL level;
}MENU_STATE;

/**
 @brief    Initialize menu initial state
 @param    void                                        
 @return   none
*/
void menuInit();


/**
 @brief    Reset menu to the initial state
 @param    void                                        
 @return   none
*/
void menuReset();

/**
 @brief    Check for menu button press and dispatch event if occurs
 @param    void                                        
 @return   none
*/
void menuPollButtons();

#endif /* MENU_H_ */