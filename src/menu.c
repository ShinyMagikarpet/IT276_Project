#include "menu.h"
#include "Player.h"
#include "save.h"
#include "gf2d_graphics.h"
#include "gf2d_element_list.h"
#include "gf2d_element_label.h"
#include "gf2d_input.h"


static Sound *menu_sound = NULL;
static Window *window = NULL;
static Element *element = NULL, *cursor = NULL;
static int _inventory = 0, _inventory_current_index = 0;
static ListElement *list = NULL;

int Pause_Menu() {

	int pause = 1, done = 0;
	if (window == NULL) {
		window = gf2d_window_load("config/pause_window.cfg");
		gf2d_window_load("config/inventory_window.cfg");
		menu_sound = gf2d_sound_load("sounds/pop.ogg", 1.0, 0);

		element = gf2d_list_get_nth(window->elements, 0);
		cursor = element->get_by_name(element, "cursor");
		ListElement *list = (ListElement *)element->data;
		slog("Element list count: %i", gf2d_list_get_count(list->list));
	}

	if (gf2d_input_command_pressed("case")) {
		Entity *player = player_get();
		Element *tempElement = element;
		element = gf2d_element_get_by_id(element, 100 + 3);
		gf2d_element_label_set_text(element, " ");
		Item *item = get_item_by_index(player->rpg.inventory[3]);
		if (item)
			item->use(player, item, 3);
		element = tempElement;
	}

	//Are we accessing our inventory?
	if (_inventory == 1) {
		Inventory();
		return pause; //End here to prevent other functions from being called
	}

	//Only way I could think of to check what the player is selecting in menu
	if (gf2d_input_command_pressed("ok")) {
		if (element) {
			switch (element->index)
			{
			case 0: {
				pause = 0;
				slog("unpaused");
				transition_window_to_black();
				pause_menu_free();
				return pause;
				break;
			}
			case 1: {
				window = get_window_get_by_id(2);
				element = gf2d_list_get_nth(window->elements, 0);
				cursor = element->get_by_name(element, "cursor");
				list = (ListElement *)element->data;
				if (!list)return;
				if (window) {
					window->state = 1;
				}
				_inventory = 1;
				_inventory_current_index = 2; //0 and 1 are reserved in the list
				
			}
			case 2: {
				SaveInfo save;
				Entity *player = player_get();
				save.player.rpg = player->rpg;
				save_write_out(save, "save/save.bin");
				break;
			}
			case 3: {
				save_load_in("save/save.bin");
				break;
			}
			case 4: {
				pause = 2;
				pause_menu_free();
				return pause;
				break;
			}
			default:
				break;
			}
		}
	}

	//move the cursor position down
	if (gf2d_input_command_pressed("move_down")) {
		move_cursor_down();
	}

	if (gf2d_input_command_pressed("move_up")) {
		move_cursor_up();
	}

	return pause;
}

void Inventory() {
	gf2d_input_update(); //Guess I need to update here or else code thinks the control calls are true
	if (gf2d_input_command_pressed("ok")) {
		window->state = 0;
		slog("why is code breaking here?");
		cursor->bounds.y -= (_inventory_current_index - 2) * 36; //current index starts at 2
		window = get_window_get_by_id(1);
		element = gf2d_list_get_nth(window->elements, 0); //Get the cursor
		cursor = element->get_by_name(element, "cursor");
		element = gf2d_list_get_nth(window->elements, 1); //spot for inventory on main menu
		
		_inventory = 0;
	}

	//move the cursor position down
	if (gf2d_input_command_pressed("move_down")) {
		move_cursor_down();
	}

	if (gf2d_input_command_pressed("move_up")) {
		move_cursor_up();
	}
}

//So this is probably the most hackey way to move my cursor. I hate this and it should burn
void move_cursor_down() { 
	int i, j, count = window->selection_count;
	Element *new_element, *new_cursor; /**< temp variable to hold info*/
	if (!element) { //If this is first time, then we know to start at 0
		new_element = gf2d_list_get_nth(window->elements, 0);
		if (!new_element)return; //check if element exists
		element = new_element;
		//cursor = new_element->get_by_name(new_element, "cursor"); //get cursor from current element
		new_element = gf2d_list_get_nth(window->elements, element->index + 1); //get next element in list
	}
	else {
		/*
		Might have to treat inventory differently because How I handled the setting up of the inventory
		in gf2d_element_list.c. The inventory list exists inside the element of inv_list and therefore would
		be best to go through that list than rewrite the entire inv_list system.
		I might just keep reference of count of where I am in the inventory list rather than point to the element itself.
		With that I can temporarily call the exact element in the list and get the info from there to know what item
		it is.
		*/
		if (_inventory) {
			if (!list)return;
			count = gf2d_list_get_count(list->list);
			if (_inventory_current_index + 1 >= count)return;
			_inventory_current_index++;
			cursor->bounds.y += 36;
			slog("Element index: %i", element->index);
			//How to get text from label
			/*
			new_element = gf2d_list_get_nth(list->list, _inventory_current_index);
			LabelElement *label = (LabelElement *)new_element->data;
			slog("label text: %s", label->text);
			*/
			gf2d_sound_play(menu_sound, 0, 1.0, -1, -1);
			return;
		}
		else {
			//If we're not starting new, then the new_element can index to the next in list
			new_element = gf2d_list_get_nth(window->elements, element->index + 1);
			if (!new_element) return;
		}
	}

	if (new_element->index >= window->selection_count)return;
	

	//new_cursor = new_element->get_by_name(new_element, "cursor"); //Get cursor from the new element
	//if (!cursor) old_element->get_by_name(old_element, "cursor");
	//if (!new_cursor)return; //If we don't have a cursor, then we reached end of list and therefore null
	for (i = new_element->index; i < count; i++) {
		if (element->index < new_element->index) {
			//cursor->state = 0; //set current cursor state to 0 to prevent draw
			//new_cursor->state = 1; //new cursor state will be set to 1 so that it can now draw
			element = new_element; // set old element to equal new element
			//cursor = new_cursor; //set old cursor information to new cursor
			break;
		}
	}
	cursor->bounds.y += 30;
	slog("Element name: %s", element->name);
	gf2d_sound_play(menu_sound, 0, 1.0, -1, -1); //Player menu sound

	/*
	I seriously hate how I did this and whoever reads this will probably get mad at me.
	Hopefully I can think of a better solution for the future or just rely on 
	another external library that could do this all for me.
	*/
}

//move_cursor_down() and everyday is opposite day
void move_cursor_up() {
	int i, j, count = gf2d_list_get_count(window->elements);
	Element *new_element, *new_cursor;

	if (!element) {
		new_element = gf2d_list_get_nth(window->elements, 0);
		element = new_element;
		//cursor = new_element->get_by_name(new_element, "cursor");
	}
	else {
		if (_inventory) {
			if (!list)return;
			count = gf2d_list_get_count(list->list);
			if (_inventory_current_index <= 2)return;
			_inventory_current_index--;
			cursor->bounds.y -= 36;
			//How to get text from label
			/*
			new_element = gf2d_list_get_nth(list->list, _inventory_current_index);
			LabelElement *label = (LabelElement *)new_element->data;
			slog("label text: %s", label->text);
			*/
			gf2d_sound_play(menu_sound, 0, 1.0, -1, -1);
			return;
		}
		else {
			new_element = gf2d_list_get_nth(window->elements, element->index - 1);
			if (!new_element)return;
		}
		
	}

	//new_cursor = new_element->get_by_name(new_element, "cursor");
	//if (!cursor) old_element->get_by_name(old_element, "cursor");
	//if (!new_cursor)return;
	for (i = new_element->index; i < count; i--) {
		if (element->index > new_element->index) {
			//cursor->state = 0;
			//new_cursor->state = 1;
			element = new_element;
			//cursor = new_cursor;
			break;
		}
	}
	//TOFIX: the channel was causing some glitch when moving cursor in both directions.
	//Moving cursor down then up would cause unexpected behavior and thus would freeze the game
	cursor->bounds.y -= 30;
	gf2d_sound_play(menu_sound, 0, 1.0, 1, -1);

}

void pause_menu_free() {
	gf2d_sound_free(menu_sound);
	//Order matters when I free windows?
	gf2d_window_free(get_window_get_by_id(2));
	gf2d_window_free(get_window_get_by_id(1));
	menu_sound = NULL;
	window = NULL;
	element = NULL;
	cursor = NULL;

}