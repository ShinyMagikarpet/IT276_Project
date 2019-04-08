#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gf2d_actor.h"
#include "gf2d_input.h"
#include <SDL_ttf.h>
#include "level.h"
#include "gf2d_draw.h"
#include "gf2d_windows.h"
#include "gf2d_elements.h"
#include "gf2d_element_label.h"
#include "gf2d_transition.h"
#include "save.h"
#include "Player.h"

static int done = 0;
static int pause = 0;

static Window *window = NULL;

int main(int argc, char * argv[])
{
    /*variable declarations*/

    const Uint8 * keys;
    Sprite *sprite;
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};
	Element *element = NULL, *cursor = NULL;
	Sound *menu_sound = NULL;

	LevelInfo *linfo = NULL;
	//Chipmunk Physics
	//cpSpace *space = gf2d_cpSpace_init();

    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        SCREENWIDTH,
        SCREENHEIGHT,
        SCREENWIDTH,
        SCREENHEIGHT,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(512);
    SDL_ShowCursor(SDL_DISABLE);
	gf2d_audio_init(256, 16, 4, 1, 1, 1);
	gf2d_windows_init(8);
	gf2d_text_init("config/font.cfg");
	gf2d_action_list_init(128);
	gf2d_entity_system_init(128);
	gf2d_input_init("config/input.cfg");
	gf2d_transition_system_init(8);

	//linfo = level_info_load("levels/overworld_2.json");
	//linfo = level_info_load("levels/overworld_1.json");
	linfo = level_info_load("levels/home_level.json");
	//linfo = level_info_load("levels/home_level.json");
	level_init(linfo, 1);

	window = gf2d_window_load("config/testwindow.cfg");
    /*demo setup*/
    //sprite = gf2d_sprite_load_image("images/menu3.png");
    //mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);

    /*main game loop*/
    while(!done)
    {
		
        //SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
		gf2d_input_update();
        
        gf2d_graphics_clear_screen();// clears drawing buffers

		//Update and draw level	
		level_draw();
		level_update();
		gf2d_windows_update_all();
		gf2d_windows_draw_all();
            //UI elements last
           /* gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);*/
		
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

		//PAUSE WHATEVER NEEDS TO BE STOPPED HERE
		if (gf2d_input_command_pressed("pause")) {
			
			slog("paused");
			pause = 1;
			transition_window_to_black();
			gf2d_window_free_all();
			window = gf2d_window_load("config/pause_window.cfg");
			gf2d_window_load("config/inventory_window.cfg");
			element = gf2d_list_get_nth(window->elements, 0);
			cursor = element->get_by_name(element, "cursor");
			menu_sound = gf2d_sound_load("sounds/pop.ogg", 1.0, 0);
			
		}

		if (gf2d_input_command_pressed("save")) {
			SaveInfo save;
			Entity *player = player_get();
			save.player.rpg = player->rpg;
			save_write_out(save, "save/save.bin");
		}

		if (gf2d_input_command_pressed("load")) {
			save_load_in("save/save.bin");
		}
		
		//Pause loop
		while (pause) {
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
				gf2d_sound_free(menu_sound);
			}
			//Quit game from pause
			if (gf2d_input_command_pressed("exit")) {
				pause = 0;
				done = 1;
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

			//Only way I could think of to check what the player is selecting in menu
			if (gf2d_input_command_pressed("ok")) {
				if (element) {
					switch (element->index)
					{
					case 0: {
						pause = 0;
						slog("unpaused");
						transition_window_to_black();
						gf2d_window_free_all();
						window = gf2d_window_load("config/testwindow.cfg");
						break;
					}
					case 1: {
						window = get_window_get_by_id(2);
						element = gf2d_list_get_nth(window->elements, 0);
						if (window) {
							window->state = 1;
						}
						slog("window element id: %i", element->index);
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
					}
					case 4: {
						pause = 0;
						done = 1;
						break;
					}
					default:
						break;
					}
				}
			}

			//move the cursor position down
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
					
				gf2d_sound_play(menu_sound, 0, 1.0, -1, -1);
				
			}

			//move the cursor up
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

				gf2d_sound_play(menu_sound, 0, 1.0, -1, -1);

			}

		}
		
		if (gf2d_input_command_pressed("exit")) {
			pause = 0;
			done = 1;
		}
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
	gf2d_input_commands_purge();
	level_info_free(linfo);
	gf2d_sound_free(menu_sound);
	cpSpaceEachShape(get_space(), (cpSpaceShapeIteratorFunc)free_all_shapes, NULL);
	cpSpaceEachBody(get_space(), (cpSpaceBodyIteratorFunc)free_all_bodies, NULL);
	level_clear();
	gf2d_window_free_all();
	//save_write_out("config/save.cfg");
    return 0;
}

/*eol@eof*/
