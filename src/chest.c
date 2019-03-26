#include "chest.h"
#include "gf2d_cpSpace.h"
#include "items.h"

void *chest_think(Entity *self);

Entity *chest_new(cpVect position, cpSpace *space) {

	Entity *chest = NULL;

	chest = gf2d_entity_new();

	//Chipmunk Physics
	chest->cpbody = cpBodyNewStatic();
	cpBodySetPosition(chest->cpbody, position);
	chest->cpbody->userData = chest;
	chest->cpshape = cpBoxShapeNew(chest->cpbody, 40, 40, 1);
	cpSpaceAddBody(space, chest->cpbody);
	cpSpaceAddShape(space, chest->cpshape);
	chest->cpshape->type = INTERACTABLE_TYPE;

	
	//Render
	vector2d_copy(chest->position, position);
	gf2d_actor_load(&chest->actor, "actor/chest.actor");
	gf2d_actor_set_action(&chest->actor, "idle");
	vector2d_copy(chest->scale, chest->actor.al->scale);

	//Info
	gf2d_line_cpy(chest->name, "chest");
	chest->rpg.selected_item = get_item_by_name("Potion")->index;
	

	//Functions
	chest->think = chest_think;

	return chest;


}

Entity *chest_spawn(cpVect position, SJson *args, cpSpace *space) {

	if (!space) {
		slog("Space not there");
		return NULL;
	}
	return chest_new(position, space);
}

void *chest_think(Entity *self) {

}