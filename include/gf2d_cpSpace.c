#include "gf2d_cpSpace.h"
#include "gf2d_entity.h"
#include "simple_logger.h"

cpSpace* gf2d_cpSpace_init(void) {

	cpSpace *newSpace = cpSpaceNew();

	//Handles Collision between player and Statics
	cpCollisionHandler *player_to_ground_handler = cpSpaceAddCollisionHandler(newSpace, PLAYER_TYPE, STATIC_TYPE);
	player_to_ground_handler->preSolveFunc = (cpCollisionPreSolveFunc)player_touch_ground;

	//Handles Collision between player and monsters
	cpCollisionHandler *player_to_monster_handler = cpSpaceAddCollisionHandler(newSpace, PLAYER_TYPE, MONSTER_TYPE);
	player_to_monster_handler->beginFunc = (cpCollisionBeginFunc)player_touch_monster_begin;
	player_to_monster_handler->preSolveFunc = (cpCollisionPreSolveFunc)player_touch_monster_presolve;

	return newSpace;

}

void *gf2d_cpSpace_update(cpSpace *space) {

	cpSpaceStep(space, SPACE_STEP);

}


static cpBool player_touch_ground(cpArbiter *arb, cpSpace *space, void *data) {
	CP_ARBITER_GET_BODIES(arb, playerbody, ground);
	//Calls player function when collision is continous
	cpTouch_player(playerbody, ground);
	return cpTrue;
}

cpBool player_touch_monster_begin(cpArbiter *arb, cpSpace *space, void *data) {
	CP_ARBITER_GET_BODIES(arb, playerbody, monster);
	//Calls player function when collision is continous
	//cpTouch_player(playerbody, monster);
	slog("TOUCHED MONSTER");
	Entity *player = (Entity *)playerbody->userData;
	Entity *other = (Entity *)monster->userData;

	player->health -= 1;

	return cpTrue;
}

cpBool player_touch_monster_presolve(cpArbiter *arb, cpSpace *space, void *data) {
	CP_ARBITER_GET_BODIES(arb, playerbody, monster);
	
	

	return cpTrue;
}