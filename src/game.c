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


int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
	Entity *player, *bug;
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};

	LevelInfo *linfo = NULL;

	//Chipmunk Physics
	cpSpace *space = gf2d_cpSpace_init();

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
   
	gf2d_action_list_init(128);
	gf2d_entity_system_init(1024);
	gf2d_input_init("config/input.cfg");
	player = player_new(cpv(200, 200), space);
	bug = bug_new(cpv(550, 360));

	linfo = level_info_load("levels/new_test..json");
	level_init(linfo, 1);
	
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);

	cpSpaceAddBody(space, bug->cpbody);
	cpSpaceAddShape(space, bug->cpshape);

	cpBody *ground = cpBodyNewStatic();
	cpShape *groundshape = cpSegmentShapeNew(ground, cpvzero, cpv(SCREENWIDTH, 0), 0);
	groundshape->type = STATIC_TYPE;
	groundshape->u = 1;
	cpSpaceAddShape(space, groundshape);


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
		gf2d_entity_think_all();
		gf2d_entity_update_all();
        
        gf2d_graphics_clear_screen();// clears drawing buffers
		// all drawing should happen betweem clear_screen and next_frame
        //backgrounds drawn first
         //gf2d_sprite_draw_image(sprite,vector2d(0,0));

		//Update and draw level	
		level_update();
		level_draw();
		//Draw Entity
		gf2d_entity_draw_all();
		gf2d_entity_draw_shape_all();
		gf2d_cpSpace_update(space);

		gf2d_draw_line(vector2d(0, 0), vector2d(SCREENWIDTH, 0), vector4d(255, 0, 0, 255));
		gf2d_draw_line(vector2d(0, 0), vector2d(0, SCREENHEIGHT), vector4d(255, 0, 0, 255));
		gf2d_draw_line(vector2d(0, SCREENHEIGHT - 1), vector2d(SCREENWIDTH -1, SCREENHEIGHT -1), vector4d(255, 0, 0, 255));
		gf2d_draw_line(vector2d(SCREENWIDTH-1, 0), vector2d(SCREENWIDTH-1, SCREENHEIGHT-1), vector4d(255, 0, 0, 255));

		
			
		

		
            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
		if (keys[SDL_SCANCODE_1]) {
			slog("XP: %i", player->rpg.xp);
		}
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
	level_info_free(linfo);

	level_clear();
	gf2d_entity_free_all();
	cpSpaceFree(space);
    return 0;
}
/*eol@eof*/
