#include "Bug.h"
#include "Player.h"
#include "gf2d_graphics.h"
#include <chipmunk_private.h>

Entity *bug = NULL;

void bug_think(Entity *self);
void bug_update(Entity *self);


Entity *bug_new(cpVect position, cpSpace *space) {

	
	bug = gf2d_entity_new();

	if (!bug) {
		slog("failed to allocate new bug");
		return;
	}
	bug->cpbody =  cpBodyNew(100, INFINITY);
	cpBodySetPosition(bug->cpbody, position);
	bug->cpbody->userData = bug;
	bug->cpshape = cpCircleShapeNew(bug->cpbody, 64, cpv(64, 64));
	cpSpaceAddBody(space, bug->cpbody);
	cpSpaceAddShape(space, bug->cpshape);
	bug->cpshape->type = MONSTER_TYPE;
	bug->cpshape->u = 1.0f;
	bug->cpshape->e = 0;
	bug->spawnpos = position;
	vector2d_copy(bug->position, position);
	gf2d_actor_load(&bug->actor, "actor/Bug.actor");
	gf2d_actor_set_action(&bug->actor, "idle");
	vector2d_copy(bug->scale, bug->actor.al->scale);
	bug->think = bug_think;
	bug->update = bug_update;
	bug->shape = gf2d_shape_circle(64, 64, 50);

	gf2d_rpg_set_stats(bug, 3, 1, 1, 1);
	bug->rpg.xp = 100;
	return bug;


}

Entity *bug_spawn(cpVect position, SJson *args, cpSpace *space)
{
	if (!space) {
		slog("Space not there");
		return NULL;
	}
	return bug_new(position, space);
}


void bug_think(Entity *self) {

	Entity *player = player_get();

	if (player) {
		
		//Get distance between player and bug
		float dirX = player->cpbody->p.x - self->cpbody->p.x;
		float dirY = player->cpbody->p.y - self->cpbody->p.y;

		//Normalize
		cpVect normal = cpvnormalize(cpv(dirX, dirY));
		if (cpvnear(cpBodyGetPosition(self->cpbody), cpBodyGetPosition(player->cpbody), 200)) {

			self->state = ES_Walk;

			if (dirX > 0)
				self->flip.x = 1;
			else
				self->flip.x = 0;
			self->cpbody->v.x = normal.x * 50.0;
			self->cpbody->v.y = normal.y * 50.0; 

		}
		else if(self->cpbody->p.x != self->spawnpos.x || self->cpbody->p.y != self->spawnpos.y){

			self->state = ES_Walk;

			//Get distance between bug current position to spawn position
			dirX = self->spawnpos.x - self->cpbody->p.x;
			dirY = self->spawnpos.y - self->cpbody->p.y;

			//Normalize
			cpVect normal = cpvnormalize(cpv(dirX, dirY));

			if (dirX > 0)
				self->flip.x = 1;
			else
				self->flip.x = 0;
			self->cpbody->v.x = normal.x * 50.0;
			self->cpbody->v.y = normal.y * 50.0;
			
			//Make sure the bug is close enough to spawn point 
			if (cpvnear(self->cpbody->p, self->spawnpos, 1))
				self->cpbody->p = self->spawnpos;

		}
		else {
			self->state = ES_Idle;
			self->cpbody->v = cpvzero;
			//slog("Returned home");
		}
			
	}
		

	self->position = cpvector_to_gf2dvector(cpBodyGetPosition(self->cpbody));
	gf2d_actor_next_frame(&self->actor);
	
}

void bug_update(Entity *self) {

	

}
