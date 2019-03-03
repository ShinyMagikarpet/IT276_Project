#include "gf2d_cpSpace.h"
#include "gf2d_entity.h"
#include "simple_logger.h"

cpSpace* gf2d_cpSpace_init(void) {

	cpSpace *newSpace = cpSpaceNew();

	//Handles Collision between player and Statics
	cpCollisionHandler *static_handler = cpSpaceAddWildcardHandler(newSpace, STATIC_TYPE);
	static_handler->preSolveFunc = (cpCollisionPreSolveFunc)touch_ground;

	//Handles Collision between player and monsters
	cpCollisionHandler *player_to_monster_handler = cpSpaceAddCollisionHandler(newSpace, PLAYER_TYPE, MONSTER_TYPE);
	player_to_monster_handler->beginFunc = (cpCollisionBeginFunc)player_touch_monster_begin;
	player_to_monster_handler->preSolveFunc = (cpCollisionPreSolveFunc)player_touch_monster_presolve;
	player_to_monster_handler->separateFunc = (cpCollisionSeparateFunc)player_touch_monster_separate;

	//Handles Collision between player and Statics
	cpCollisionHandler *monster_to_monster_handler = cpSpaceAddCollisionHandler(newSpace, MONSTER_TYPE, MONSTER_TYPE);
	monster_to_monster_handler->beginFunc = (cpCollisionBeginFunc)monster_touch_monster_begin;

	return newSpace;

}

void *gf2d_cpSpace_update(cpSpace *space) {

	cpSpaceStep(space, SPACE_STEP);

}


static cpBool touch_ground(cpArbiter *arb, cpSpace *space, void *data) {
	

	
	return cpTrue;
}

cpBool player_touch_monster_begin(cpArbiter *arb, cpSpace *space, void *data) {
	CP_ARBITER_GET_BODIES(arb, playerbody, monster);
	//Calls player function when collision is continous
	//cpTouch_player(playerbody, monster);
	slog("TOUCHED MONSTER");
	Entity *player = (Entity *)playerbody->userData;
	Entity *other = (Entity *)monster->userData;

	//Handles damage and invincibility frames here
	if (player->iframes <= 0) {
		player->rpg.stats.hp -= 1;
		player->iframes = 120;
	}

	if (player->rpg.stats.hp == 0) {
		slog("PLAYER SHOULD DIE");
	}

	return cpTrue;
}

cpBool player_touch_monster_presolve(cpArbiter *arb, cpSpace *space, void *data) {
	CP_ARBITER_GET_BODIES(arb, playerbody, monster);
	CP_ARBITER_GET_SHAPES(arb, playershape, monstershape);

	cpContactPointSet set = cpArbiterGetContactPointSet(arb);

	//Knockback player after getting hit
	cpArbiterSetSurfaceVelocity(arb, cpv(2000 * -set.normal.x, 2000 * -set.normal.y));
	
	return cpTrue;
}

void player_touch_monster_separate(cpArbiter *arb, cpSpace *space, void *data) {
	CP_ARBITER_GET_BODIES(arb, playerbody, monster);
	CP_ARBITER_GET_SHAPES(arb, playershape, monstershape);


	cpBodySetVelocity(monster, cpvzero);

}


cpBool monster_touch_monster_begin(cpArbiter *arb, cpSpace *space, void *data) {

	return cpFalse;
}