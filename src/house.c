#include "house.h"
#include "Player.h"

Entity *house_new(Vector2D position,cpSpace *space, const char *nextLevel, const char *target, Uint32 id);

Entity *house_new(cpVect position, cpSpace *space, const char *nextLevel, const char *target, Uint32 id) {
	Entity *house = gf2d_entity_new();
	
	//Chipmunk Physics
	house->cpbody = cpBodyNewStatic();
	house->cpbody->userData = house;
	cpSpaceAddBody(space, house->cpbody);


	//House borders

	//Left side
	house->cpshape = cpSegmentShapeNew(house->cpbody, cpv(position.x + 10, position.y + 50), cpv(position.x + 10, position.y + 176), 1);
	cpSpaceAddShape(space, house->cpshape);
	
	//Bottom left
	house->cpshape = cpSegmentShapeNew(house->cpbody, cpv(position.x + 10, position.y + 176), cpv(position.x + 50, position.y + 176), 1);
	cpSpaceAddShape(space, house->cpshape);
	
	//Top
	house->cpshape = cpSegmentShapeNew(house->cpbody, cpv(position.x + 10, position.y + 50), cpv(position.x + 130, position.y + 50), 1);
	cpSpaceAddShape(space, house->cpshape);
	
	//Right side
	house->cpshape = cpSegmentShapeNew(house->cpbody, cpv(position.x + 130, position.y + 50), cpv(position.x + 130, position.y + 176), 1);
	cpSpaceAddShape(space, house->cpshape);

	//Bottom Right
	house->cpshape = cpSegmentShapeNew(house->cpbody, cpv(position.x + 130, position.y + 176), cpv(position.x + 90, position.y + 176), 1);
	cpSpaceAddShape(space, house->cpshape);

	//Door transition
	house->cpshape = cpSegmentShapeNew(house->cpbody, cpv(position.x + 50, position.y + 150), cpv(position.x + 90, position.y + 150), 1);
	house->cpshape->type = TRANSITION_TYPE;
	house->cpshape->userData = house;
	cpSpaceAddShape(space, house->cpshape);
	


	//house->cpshape->type = TRANSITION_TYPE;
	//Render
	vector2d_copy(house->position, position);
	gf2d_actor_load(&house->actor, "actors/house.actor");
	gf2d_actor_set_action(&house->actor, "idle");
	vector2d_copy(house->scale, house->actor.al->scale);
	
	//house->shape = gf2d_shape_rect(0, 0, 140, 176);
	//Info
	gf2d_line_cpy(house->name, "house");
	house->targetId = id;
	gf2d_line_cpy(house->targetLevel, nextLevel);
	gf2d_line_cpy(house->targetEntity, target);

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

