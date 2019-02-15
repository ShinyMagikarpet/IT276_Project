#ifndef __GF2D_INPUT_H__
#define __GF2D_INPUT_H__

#include <SDL.h>
#include "gf2d_text.h"
#include "gf2d_list.h"

typedef enum {

		IE_Idle = 0,
		IE_Pressed = 1,
		IE_Hold = 2,
		IE_Release = 3

}InputEvent;

typedef struct {

	TextLine command;
	List *keyCodes;
	int numKeyCodes;
	Uint32 pressTime;
	InputEvent state;
	void(*onPress)(void *data);
	void(*onHold)(void *data);
	void(*onRelease)(void *data);
	void(*onDelete)(void *data);
	void *data;

}Input;

void gf2d_input_init(char *configFile);

/**
 * @brief called as often as you want your input data update.  Every Frame, or faster if you like
 * @note must be called or input will be stale
 */
void gf2d_input_update();

/**
 * @brief load user configuration from file.
 * @note this operation appends to existing commands
 */
void gf2d_input_commands_load(char *configFile);

/**
 * @brief clears all user input configuration
 */
void gf2d_input_commands_purge();

/**
 * @brief check if a command was pressed this frame
 * @param command the name of the command to check
 * @returns true if pressed, false otherwise
 */
Uint8 gf2d_input_command_pressed(const char *command);
Uint8 gf2d_input_command_held(const char *command);
Uint8 gf2d_input_command_released(const char *command);
Uint8 gf2d_input_command_down(const char *command);

InputEvent gf2d_input_command_get_state(const char *command);

/**
 * @brief report if the key provided has been pressed this frame
 * @param key the name of the key to check
 * @returns true if pressed, false otherwise
 *
 */
Uint8 gf2d_input_key_pressed(const char *key);
Uint8 gf2d_input_key_released(const char *key);
Uint8 gf2d_input_key_held(const char *key);
Uint8 gf2d_input_key_down(const char *key);

/**
 * @brief configure callbacks for an input
 * @param command the name of the input to configure
 * @param onPress the function to call when the input is pressed
 * @param onHold the function to call when the input is held
 * @param onRelease the function to call when the input is released
 * @param onDelete the function to call when the input is deleted from memory to clean up
 * @param data pointer to any custom data you want associated with the input
 */
void gf2d_input_set_callbacks(
	char *command,
	void(*onPress)(void *data),
	void(*onHold)(void *data),
	void(*onRelease)(void *data),
	void(*onDelete)(void *data),
	void *data
);

#endif // !__GF2D_INPUT_H__
