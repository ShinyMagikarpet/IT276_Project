#include <SDL.h>
#include <chipmunk.h>
#include <chipmunk_types.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "Player.h"
#include "Bug.h"
#include "gf2d_actor.h"
#include "gf2d_input.h"
#include <chipmunk_private.h>
#include <SDL_ttf.h>
#include "level.h"
#include "gf2d_cpSpace.h"
#include "gf2d_draw.h"
#include "camera.h"
#include "gf2d_windows.h"

static int done = 0;
static int pause = 0;

static Window *window = NULL;

int main(int argc, char * argv[])
{
    /*variable declarations*/

    const Uint8 * keys;
    Sprite *sprite;
	Entity *player, *bug;
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};

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
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
	gf2d_audio_init(256, 16, 4, 1, 1, 1);
	gf2d_windows_init(128);
	gf2d_text_init("config/font.cfg");
	gf2d_action_list_init(128);
	gf2d_entity_system_init(1024);
	gf2d_input_init("config/input.cfg");

	//Deprecated as it is now being done in level
	//player = player_new(cpv(200, 200), space);
	//bug = bug_new(cpv(550, 360), space);

	linfo = level_info_load("levels/new_test..json");
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
		//gf2d_entity_think_all();
		//gf2d_entity_update_all();
        
        gf2d_graphics_clear_screen();// clears drawing buffers
		// all drawing should happen betweem clear_screen and next_frame
        //backgrounds drawn first
         //gf2d_sprite_draw_image(sprite,vector2d(0,0));

		//Update and draw level	
		level_update();
		level_draw();
		//Draw Entity
		//gf2d_entity_draw_all();
		gf2d_windows_update_all();
		gf2d_windows_draw_all();
		//Deprecated as it is now updated in level
		//gf2d_cpSpace_update(space);
		//gf2d_sprite_draw_image(sprite, vector2d(0, SCREENHEIGHT - 160), vector2d(1, 1));
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
			gf2d_window_free_all(); //may need to use free all in the future
			window = gf2d_window_load("config/pause_window.cfg");
			window = gf2d_window_load("config/pause_stats.cfg");
			
		}
			
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
			}
			//Quit game from pause
			if (gf2d_input_command_pressed("exit")) {
				pause = 0;
				done = 1;
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
	level_info_free(linfo);

	level_clear();
	gf2d_entity_free_all();
	gf2d_window_free_all();
    return 0;
}
/*eol@eof*/
