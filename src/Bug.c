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
	bug->cpbody =  cpBodyNew(100000, INFINITY);
	cpBodySetPosition(bug->cpbody, cpv(position.x + 64, position.y + 64));
	bug->cpbody->userData = bug;
	bug->cpshape = cpBoxShapeNew(bug->cpbody, 128, 128, 1);
	bug->cpshape->type = MONSTER_TYPE;
	bug->cpshape->u = 1.0f;
	bug->cpshape->e = 0;

	vector2d_copy(bug->position, position);
	gf2d_actor_load(&bug->actor, "actor/Bug.actor");
	gf2d_actor_set_action(&bug->actor, "idle");
	bug->think = bug_think;
	bug->update = bug_update;
	bug->shape = gf2d_shape_rect(0, 0, 128, 128);
	return bug;


}


void bug_think(Entity *self) {

	Entity *player = player_get();

	/*if (player) {
		if (cpvnear(cpBodyGetPosition(self->cpbody), cpBodyGetPosition(player->cpbody), 200))
			slog("BUG NEAR PLAYER!!!");
	}*/


	if(cpSpaceSegmentQueryFirst(self->cpbody->space, cpv(self->cpbody->p.x + 10, self->cpbody->p.y), cpv(self->cpbody->p.x + 10, self->cpbody->p.y) , 1, CP_SHAPE_FILTER_NONE , NULL))
		slog("WAAAAAAAAAAAAAAAh");
		
	
}

void bug_update(Entity *self) {

	self->position = cpvector_to_gf2dvector(cpBodyGetPosition(self->cpbody));
	gf2d_actor_next_frame(&bug->actor);

}
