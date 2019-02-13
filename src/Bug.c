#include "Bug.h"

void bug_think(Entity *self);
void bug_update(Entity *self);

Entity *bug_new(Vector2D position, Entity *player) {

	Entity *bug = NULL;
	bug = gf2d_entity_new();

	if (!bug) {
		slog("failed to allocate new bug");
		return;
	}

	vector2d_copy(bug->position, position);
	bug->target = player;
	bug->sprite = gf2d_sprite_load_all("images/space_bug.png", 128, 128, 16);
	bug->frame = 48;
	bug->update = bug_update;

	slog("allocated new bug");
	return bug;


}


void bug_think(Entity *self) {

}

void bug_update(Entity *self) {

	Vector2D dir;
	if (!self)return;
	if (!self->target)return;

	vector2d_sub(dir, self->target->position, self->position);
	vector2d_normalize(&dir);
	vector2d_copy(self->velocity, dir);

}
