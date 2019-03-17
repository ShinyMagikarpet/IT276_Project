#include "house.h"
#include "Player.h"

Entity *house_new(Vector2D position,cpSpace *space, const char *nextLevel, const char *target, Uint32 id);

Entity *house_new(cpVect position, cpSpace *space, const char *nextLevel, const char *target, Uint32 id) {
	Entity *house = gf2d_entity_new();

	//Chipmunk Physics
	house->cpbody = cpBodyNewStatic();
	cpBodySetPosition(house->cpbody, position);
	house->cpbody->userData = house;
	house->cpshape = cpCircleShapeNew(house->cpbody, 64, cpv(68, 64));
	cpSpaceAddBody(space, house->cpbody);
	cpSpaceAddShape(space, house->cpshape);
	house->cpshape->type = MONSTER_TYPE;

	//Render
	vector2d_copy(house->position, position);
	gf2d_actor_load(&house->actor, "actor/house.actor");
	gf2d_actor_set_action(&house->actor, "idle");
	vector2d_copy(house->scale, house->actor.al->scale);

	//Info
	gf2d_line_cpy(house->name, "Home");
	house->targetId = id;

	//TODO: FINISH HOUSE ENTITY AND BE ABLE TO LEVEL TRANSITION

	return house;
}

Entity *house_spawn(cpVect position, SJson *args, cpSpace *space) {
	int id = 0;
	if (!space)return NULL;
	if (!args)return NULL;
	sj_get_integer_value(sj_object_get_value(args, "targetId"), &id);
	return house_new(position,
		space,
		sj_get_string_value(sj_object_get_value(args, "level")),
		sj_get_string_value(sj_object_get_value(args, "target")),
		id);
}

