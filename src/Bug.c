#include "Bug.h"
#include "Player.h"
#include "gf2d_graphics.h"
#include <chipmunk_private.h>

Entity *bug = NULL;

void bug_think(Entity *self);
void bug_update(Entity *self);


Entity *bug_new(cpVect position) {

	
	bug = gf2d_entity_new();

	if (!bug) {
		slog("failed to allocate new bug");
		return;
	}
	bug->cpbody =  cpBodyNew(100, INFINITY);
	cpBodySetPosition(bug->cpbody, position);
	bug->cpbody->userData = bug;
	bug->cpshape = cpCircleShapeNew(bug->cpbody, 50, cpv(64, 64));
	bug->cpshape->type = MONSTER_TYPE;
	bug->cpshape->u = 1.0f;
	bug->cpshape->e = 0;
	bug->cpPos = position;

	vector2d_copy(bug->position, position);
	gf2d_actor_load(&bug->actor, "actor/Bug.actor");
	gf2d_actor_set_action(&bug->actor, "idle");
	bug->think = bug_think;
	bug->update = bug_update;
	bug->shape = gf2d_shape_circle(64, 64, 50);

	bug->rpg.stats.hp = 3;
	bug->rpg.xp = 10;
	return bug;


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

			/*if (dirX > 0)
				self->flip.x = 1;
			else
				self->flip.x = 0;
			self->cpbody->v.x = dirX * 0.3;
			self->cpbody->v.y = dirY * 0.3;*/;

		}
		else if(self->cpbody->p.x != self->cpPos.x || self->cpbody->p.y != self->cpPos.y){

			//Get distance between player and bug
			dirX = self->cpPos.x - self->cpbody->p.x;
			dirY = self->cpPos.y - self->cpbody->p.y;

			//Normalize
			cpVect normal = cpvnormalize(cpv(dirX, dirY));

			if (dirX > 0)
				self->flip.x = 1;
			else
				self->flip.x = 0;
			self->cpbody->v.x = dirX * 0.3;
			self->cpbody->v.y = dirY * 0.3;
			if(cpvnear(cpBodyGetPosition(self->cpbody), self->cpPos, 5))
				self->cpbody->p = self->cpPos;
		}
		else {
			self->cpbody->v = cpvzero;
		}
			
	}
		
	
}

void bug_update(Entity *self) {

	self->position = cpvector_to_gf2dvector(cpBodyGetPosition(self->cpbody));
	gf2d_actor_next_frame(&bug->actor);

}
