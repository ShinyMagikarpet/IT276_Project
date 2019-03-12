#include "gf2d_cpSpace.h"
#include "gf2d_entity.h"
#include "gf2d_graphics.h"
#include "simple_logger.h"

cpSpace* gf2d_cpSpace_init(void) {

	cpSpace *newSpace = cpSpaceNew();

	setup_boundaries(newSpace);

	//Handles Collision between player and Statics
	cpCollisionHandler *static_handler = cpSpaceAddWildcardHandler(newSpace, STATIC_TYPE);
	static_handler->preSolveFunc = (cpCollisionPreSolveFunc)touch_ground;

	//Handles Collision between player and monsters
	cpCollisionHandler *player_to_monster_handler = cpSpaceAddCollisionHandler(newSpace, PLAYER_TYPE, MONSTER_TYPE);
	player_to_monster_handler->beginFunc = (cpCollisionBeginFunc)player_touch_monster_begin;
	player_to_monster_handler->preSolveFunc = (cpCollisionPreSolveFunc)player_touch_monster_presolve;
	player_to_monster_handler->postSolveFunc = (cpCollisionPostSolveFunc)player_touch_monster_postsolve;
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

	Entity *player = (Entity *)playerbody->userData;
	Entity *other = (Entity *)monster->userData;

	//TODO: Find better way to check if to ignore damage + knockback
	if (strcmp(other->name, "bug_hive") == 0) {
		return cpTrue;
	}

	//Handles damage and invincibility frames here
	if (player->iframes <= 0) {
		player->rpg.stats.hp_current -= 1;
		player->iframes = 120;
	}

	if (player->rpg.stats.hp_current == 0) {
		slog("PLAYER SHOULD DIE");
	}

	return cpTrue;
}

cpBool player_touch_monster_presolve(cpArbiter *arb, cpSpace *space, void *data) {
	CP_ARBITER_GET_BODIES(arb, playerbody, monster);
	CP_ARBITER_GET_SHAPES(arb, playershape, monstershape);

	Entity *player = (Entity *)playerbody->userData;
	Entity *other = (Entity *)monster->userData;

	if (strcmp(other->name, "bug_hive") == 0) {
		return cpTrue;
	}

	cpContactPointSet set = cpArbiterGetContactPointSet(arb);

	//Knockback player after getting hit
	cpArbiterSetSurfaceVelocity(arb, cpv(2500 * -set.normal.x, 2500 * -set.normal.y));
	
	monster->v = cpvzero;
	return cpTrue;
}

void player_touch_monster_postsolve(cpArbiter *arb, cpSpace *space, void *data) {

	Entity *inflicted = (Entity *)data;
	
	if (inflicted) {
		//self->rpg.xp += inflicted->rpg.xp;
		cpSpaceRemoveShape(inflicted->cpbody->space, inflicted->cpshape);
		cpShapeFree(inflicted->cpshape);
		cpSpaceRemoveBody(inflicted->cpbody->space, inflicted->cpbody);
		cpBodyFree(inflicted->cpbody);
		gf2d_entity_free(inflicted);
	}
	

	
}

void player_touch_monster_separate(cpArbiter *arb, cpSpace *space, void *data) {
	CP_ARBITER_GET_BODIES(arb, playerbody, monster);
	CP_ARBITER_GET_SHAPES(arb, playershape, monstershape);

	//TODO: make counter for when monster attacks player as a cooldown so that they don't move
	//after they touch the player
	cpBodySetVelocity(monster, cpvzero);

}


cpBool monster_touch_monster_begin(cpArbiter *arb, cpSpace *space, void *data) {

	return cpFalse;
}

void setup_boundaries(cpSpace *space) {

	cpBody *ground = cpBodyNewStatic();
	cpShape *groundshape = cpSegmentShapeNew(ground, cpvzero, cpv(1600, 0), 1);
	groundshape->type = STATIC_TYPE;
	groundshape->u = 1;
	cpSpaceAddShape(space, groundshape);

	groundshape = cpSegmentShapeNew(ground, cpv(0, 1600), cpv(1600, 1600), 1);
	groundshape->type = STATIC_TYPE;
	groundshape->u = 1;
	cpSpaceAddShape(space, groundshape);

	groundshape = cpSegmentShapeNew(ground, cpvzero, cpv(0, 1600), 1);
	groundshape->type = STATIC_TYPE;
	groundshape->u = 1;
	cpSpaceAddShape(space, groundshape);

	groundshape = cpSegmentShapeNew(ground, cpv(1600, 0), cpv(1600, 1600), 1);
	groundshape->type = STATIC_TYPE;
	groundshape->u = 1;
	cpSpaceAddShape(space, groundshape);

}

/*eol@eof*/