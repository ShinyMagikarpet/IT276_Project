#include "level.h"
#include "gf2d_windows.h"
#include "gf2d_elements.h"
#include "gf2d_input.h"

int pause = 0;

int pause_main() {


	Window *window = gf2d_window_load("config/testwindow.cfg");
	window = gf2d_window_load("config/pause_window.cfg");
	Element *element = gf2d_list_get_nth(window->elements, 0);
	Element *cursor = element->get_by_name(element, "cursor");
	int done = 0;

	while (!done) {

		gf2d_input_update();
		gf2d_windows_update_all();
		gf2d_windows_draw_all();
		gf2d_grahics_next_frame();


		if (gf2d_input_command_pressed("pause")) {
			pause = 0;
			slog("unpaused");
			transition_window_to_black();
			gf2d_window_free_all();
			window = gf2d_window_load("config/testwindow.cfg");
		}
		//Quit game from pause
		if (gf2d_input_command_pressed("exit")) {
			done = 1;
		}

		if (gf2d_input_command_pressed("ok")) {
			if (element) {
				slog("Element name: %s", element->name);
			}
		}

		if (gf2d_input_command_pressed("move_down")) {
			int i, j, count = gf2d_list_get_count(window->elements);
			Element *new_element, *new_cursor;

			if (!element) {
				new_element = gf2d_list_get_nth(window->elements, 0);
				element = new_element;
				cursor = new_element->get_by_name(new_element, "cursor");
				new_element = gf2d_list_get_nth(window->elements, element->index + 1);
			}

			else {
				new_element = gf2d_list_get_nth(window->elements, element->index + 1);
			}


			new_cursor = new_element->get_by_name(new_element, "cursor");
			//if (!cursor) old_element->get_by_name(old_element, "cursor");
			if (!new_cursor)break;
			for (i = new_element->index; i < count; i++) {
				if (element->index < new_element->index) {
					cursor->state = 0;
					new_cursor->state = 1;
					element = new_element;
					cursor = new_cursor;
					break;
				}
			}


		}

		if (gf2d_input_command_pressed("move_up")) {
			int i, j, count = gf2d_list_get_count(window->elements);
			Element *new_element, *new_cursor;

			if (!element) {
				new_element = gf2d_list_get_nth(window->elements, 0);
				element = new_element;
				cursor = new_element->get_by_name(new_element, "cursor");
			}

			else {
				new_element = gf2d_list_get_nth(window->elements, element->index - 1);
				if (!new_element)break;
			}


			new_cursor = new_element->get_by_name(new_element, "cursor");
			//if (!cursor) old_element->get_by_name(old_element, "cursor");
			if (!new_cursor)break;
			for (i = new_element->index; i < count; i--) {
				if (element->index > new_element->index) {
					cursor->state = 0;
					new_cursor->state = 1;
					element = new_element;
					cursor = new_cursor;
					break;
				}
			}

			return done;
		}
	}
	
	return done;
}