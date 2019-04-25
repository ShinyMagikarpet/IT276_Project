#include "gf2d_cpSpace.h"
#include "gf2d_graphics.h"
#include "simple_logger.h"
#include "level.h"
#include "gf2d_windows.h"
#include "gf2d_entity_common.h"
#include "Player.h"
#include "gf2d_transition.h"

static cpSpace *newSpace = NULL;



cpSpace* gf2d_cpSpace_init(void) {

	newSpace = cpSpaceNew();

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

	//Handles Collision between player and interactibles
	//cpCollisionHandler *player_to_transition_handler = cpSpaceAddCollisionHandler(newSpace, PLAYER_TYPE, INTERACTABLE_TYPE);
	//player_to_transition_handler->beginFunc = (cpCollisionBeginFunc)player_touch_interactiable_begin;
	//return newSpace;
}

cpSpace *get_space() {
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
		player->rpg.stats.hp_current -= other->damage(other, player);
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

		cpSpaceAddPostStepCallback(space, (cpPostStepFunc)free_physics, inflicted, NULL);

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


cpBool player_touch_transition_begin(cpArbiter *arb, cpSpace *space, void *data) {
	
	CP_ARBITER_GET_SHAPES(arb, playershape, transitionershape);
	
	Transition *transitiondata = (Transition *)transitionershape->userData;
	if (strcmp(transitiondata->targetLevel, "") == 0) {
		slog("Level wasn't setup properly.Name wasn't found");
		return;
	}

	cpSpaceAddPostStepCallback(space, (cpPostStepFunc)post_step_remove, transitionershape, NULL);
	
	//This is the biggest hack I've done so far in my opnion.
	//Doing this prevents the callback being called twice and giving
	//the function unusable garbage, which was causing crashes
	if (space->postStepCallbacks->num > 1)
		space->postStepCallbacks->num = 1;
	
	//transition_window_to_black();
		
	
	
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


void free_physics(cpSpace *space, Entity *self, void *data) {


	cpSpaceRemoveShape(space, self->cpshape);
	cpSpaceRemoveBody(space, self->cpbody);
	cpShapeFree(self->cpshape);
	//self->cpshape = NULL;
	cpBodyFree(self->cpbody);
	//self->cpbody = NULL;
	gf2d_entity_free(self);
	
}

void free_all_shapes(cpShape *shape, void *data) {
	cpShapeDestroy(shape);
	cpShapeFree(shape);
	shape = NULL;
}

void free_all_bodies(cpBody *body, void *data) {
	cpBodyDestroy(body);
	cpBodyFree(body);
	body = NULL;
}

void post_step_remove(cpSpace *space, cpShape *shape, void *data) {
	TextLine targetLevel;  //, targetEntity;
	Uint32 targetId;
	cpVect player_spawn_pos;
	Entity *player = player_get();

	Transition *transitiondata = (Transition *)shape->userData;
	if (!transitiondata)return;
	if (!player)return;
	if (!space)return;
	gf2d_line_cpy(targetLevel, transitiondata->targetLevel);
	slog("player Pos: %f, %f", transitiondata->player_pos.x, transitiondata->player_pos.y);
	player_spawn_pos = transitiondata->player_pos;
	//gf2d_line_cpy(targetEntity, "house");
	targetId = 1;
	transition_window_to_black();
	cpSpaceEachShape(space, (cpSpaceShapeIteratorFunc)free_all_shapes, NULL);
	cpSpaceEachBody(space, (cpSpaceBodyIteratorFunc)free_all_bodies, NULL);
	entity_clear_all_but_player();
	player->free(player);
	level_transition(targetLevel, NULL, targetId);
	slog("player Pos: %f, %f", transitiondata->player_pos.x, transitiondata->player_pos.y);
	if(player_spawn_pos.x != 0 && player_spawn_pos.y != 0)
		player->cpbody->p = player_spawn_pos;
	
}

/*eol@eof*/