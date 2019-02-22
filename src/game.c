#include <SDL.h>
#include <chipmunk.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "Player.h"
#include "Bug.h"
#include "gf2d_actor.h"
#include "gf2d_input.h"

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
	cpSpace *space = cpSpaceNew();
	cpVect gravity = cpv(0, -100);
	cpFloat timestep = 1 / 60;
	cpSpaceSetGravity(space, gravity);

    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
   
	gf2d_action_list_init(128);
	gf2d_entity_system_init(1024);
	gf2d_input_init("config/input.cfg");
	player = player_new(vector2d(600, 360));
	bug = bug_new(cpv(500, 400));

    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
	cpSpaceAddBody(space, player->cpbody);
	cpSpaceAddShape(space, player->cpshape);
	cpSpaceAddBody(space, bug->cpbody);
	cpSpaceAddShape(space, bug->cpshape);
	cpShape *ground = cpSegmentShapeNew(cpSpaceGetStaticBody(space), cpv(-20, 5), cpv(20, -5), 0);
	cpShapeSetFriction(ground, 1);
	cpSpaceAddShape(space, ground);

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
         gf2d_sprite_draw_image(sprite,vector2d(0,0));
			

		//Draw Entity
		gf2d_entity_draw_all();

		cpBodyUpdateVelocity(bug->cpbody, gravity, 0.1, 1);
		cpSpaceStep(space, timestep);

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
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
	gf2d_entity_free(player);
	gf2d_entity_free(bug);
	cpSpaceFree(space);
    return 0;
}
/*eol@eof*/
