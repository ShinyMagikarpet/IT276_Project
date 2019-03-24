#include "bug_hive.h"
#include "Player.h"
#include "Bug.h"
#include "gf2d_graphics.h"
#include <chipmunk_private.h>
#include "gf2d_cpSpace.h"


void bug_hive_think(Entity *self);

Entity *bug_hive_new(cpVect position, cpSpace *space) {

	Entity *bug_hive = NULL;

	bug_hive = gf2d_entity_new();

	//Chipmunk Physics
	bug_hive->cpbody = cpBodyNewStatic();
	cpBodySetPosition(bug_hive->cpbody, position);
	bug_hive->cpbody->userData = bug_hive;
	bug_hive->cpshape = cpCircleShapeNew(bug_hive->cpbody, 64, cpv(68, 64));
	cpSpaceAddBody(space, bug_hive->cpbody);
	cpSpaceAddShape(space, bug_hive->cpshape);
	bug_hive->cpshape->type = MONSTER_TYPE;

	//Render
	vector2d_copy(bug_hive->position, position);
	gf2d_actor_load(&bug_hive->actor, "actor/bug_hive.actor");
	gf2d_actor_set_action(&bug_hive->actor, "idle");
	vector2d_copy(bug_hive->scale, bug_hive->actor.al->scale);

	//Info
	gf2d_line_cpy(bug_hive->name, "bug_hive");
	gf2d_rpg_set_stats(bug_hive, 15, 0, 1, 0);
	bug_hive->cooldown = 0;
	bug_hive->rpg.xp = 1000;

	//Functions
	bug_hive->think = bug_hive_think;

	return bug_hive;
}

Entity *bug_hive_spawn(cpVect position, SJson *args, cpSpace *space) {

	if (!space) {
		slog("Space not there");
		return NULL;
	}
	return bug_hive_new(position, space);
}

void bug_hive_spawn_bug(Entity *self) {

	if (!self)return;
	cpVect position = self->cpbody->p;
	
	Entity *bug = bug_spawn(cpv(position.x + 30, position.y + 55), NULL, self->cpbody->space);
}

void bug_hive_think(Entity *self) {
	if (self->cooldown > 0)
		self->cooldown -= 0.1;
	else
		self->cooldown = 0;


	Entity *player = player_get();

	if (player == NULL)return;
	if (cpvnear(cpBodyGetPosition(self->cpbody), cpBodyGetPosition(player->cpbody), 250) && self->cooldown == 0) {

		bug_hive_spawn_bug(self);
		self->cooldown = 10000;

	}
}
