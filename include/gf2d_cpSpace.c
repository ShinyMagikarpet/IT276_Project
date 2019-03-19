#include "gf2d_cpSpace.h"
#include "gf2d_graphics.h"
#include "simple_logger.h"
#include "level.h"
#include "gf2d_windows.h"
#include "gf2d_entity_common.h"
#include "Player.h"

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

	//Handles Collision between player and shapes meant for level transition
	cpCollisionHandler *player_to_transition_handler = cpSpaceAddCollisionHandler(newSpace, PLAYER_TYPE, TRANSITION_TYPE);
	player_to_transition_handler->beginFunc = (cpCollisionBeginFunc)player_touch_transition_begin;
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
		player->iframes = 2.0;
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
	playerbody->p = cpvadd(playerbody->p, cpv(50 * -set.normal.x, 50 * -set.normal.y));
	//cpArbiterSetSurfaceVelocity(arb, cpv(2500 * -set.normal.x, 2500 * -set.normal.y));
	
	monster->v = cpvzero;
	return cpTrue;
}

void player_touch_monster_postsolve(cpArbiter *arb, cpSpace *space, void *data) {

	Entity *inflicted = (Entity *)data;
	
	if (inflicted) {
		//self->rpg.xp += inflicted->rpg.xp;
		gf2d_entity_free_physics(inflicted);
		gf2d_entity_free(inflicted);
		//cpSpaceAddPostStepCallback(space, (cpPostStepFunc)post_step_remove, NULL, NULL);
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


void player_touch_transition_begin(cpArbiter *arb, cpSpace *space, void *data) {
	
	slog("Transition Time!!!");
	CP_ARBITER_GET_BODIES(arb, playerbody, transitioner);
	CP_ARBITER_GET_SHAPES(arb, playershape, transitionershape);
	Entity *player = (Entity *)playerbody->userData;
	Entity *other = (Entity *)transitioner->userData;

	if (!player)return;
	if (!other)return;

	cpSpaceAddPostStepCallback(space, (cpPostStepFunc)post_step_remove, transitionershape, NULL);
	transition_window_to_black();
		
	
	
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

void post_step_remove(cpSpace *space, cpShape *shape, void *data) {
	TextLine targetLevel, targetEntity;
	Uint32 targetId;
	Entity *player = player_get();
	Entity *other = (Entity *)shape->userData;
	if (!other)return;
	if (!player)return;

	gf2d_line_cpy(targetLevel, other->targetLevel);
	gf2d_line_cpy(targetEntity, other->targetEntity);
	targetId = other->targetId;

	entity_clear_all_but_player();
	gf2d_entity_free_physics(player);
	player->free(player);

	level_transition(targetLevel, targetEntity, targetId);


}

/*eol@eof*/