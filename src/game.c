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
#include "menu.h"

static int _done = 0;
static int _pause = 0;
static int _main_menu = 1;

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
	int _editor;
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
	gf2d_entity_system_init(256);
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
   GAME_LOOP: while(!_done)
   {
		//sort_entities();
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
		if (!_pause && !_main_menu) {
			level_update();
		}
			
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
		if (gf2d_input_command_pressed("pause") && !_main_menu) {
			_pause = 1;
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

		if (_main_menu) {
			_main_menu = Main_Menu();
			if (_main_menu == 1)
				_editor = 1;
			else if (_main_menu == 2) {
				_main_menu = 0;
				_done = 1;
			}

		}



		//Pause loop
		if (_pause) {
			_pause = Pause_Menu();
			if (_pause == 2) {
				_pause = 0;
				_done = 1;
			}
		}
		
		if (gf2d_input_command_pressed("exit")) {
			_pause = 0;
			_done = 1;
		}
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
   }
    slog("---==== END ====---");
	gf2d_input_commands_purge();
	level_info_free(linfo);
	cpSpaceEachShape(get_space(), (cpSpaceShapeIteratorFunc)free_all_shapes, NULL);
	cpSpaceEachBody(get_space(), (cpSpaceBodyIteratorFunc)free_all_bodies, NULL);
	level_clear();
	gf2d_window_free_all();
	//save_write_out("config/save.cfg");
    return 0;
}

/*eol@eof*/
