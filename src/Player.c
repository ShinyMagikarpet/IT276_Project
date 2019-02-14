#include "Player.h"
#include "simple_logger.h"

void player_think(Entity *self);
void player_update(Entity *self);
float mf = 0;

Entity *player_new(Vector2D position) {

	Entity *player = NULL;

	player = gf2d_entity_new();

	if (!player) {
		slog("failed to allocate new player");
		return NULL;
	}

	vector2d_copy(player->position, position);
	player->sprite = gf2d_sprite_load_all("images/character.png", 16, 32, 4);
	player->frame = 0;
	player->dir = DIR_DOWN;
	player->scale.x = 2;
	player->scale.y = 2;
	player->think = player_think;
	player->update = player_update;
	return player;

}

void player_think(Entity *self) {

}

void player_walk_frame_range(Entity *self, int frameStart, int frameEnd) {

	if (self->frame >= frameEnd + 1) {
		self->frame = frameStart;
	}
	else {
		if (mf == 0)
			self->frame++;
	}
	if (self->frame > frameEnd || self->frame < frameStart)self->frame = frameStart;


}

void player_update(Entity *self) {


	Bool isButtonPressed = false;
	const Uint8 *keys;

	if (mf >= 1.0)mf = 0;
	keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_UP]) {
		isButtonPressed = true;
		self->dir = DIR_UP;
		self->position.y -= 2;

		player_walk_frame_range(self, 8, 11);
		
	}
	if (keys[SDL_SCANCODE_DOWN]) {
		isButtonPressed = true;
		self->dir = DIR_DOWN;
		self->position.y += 2;

		player_walk_frame_range(self, 0, 3);
		
		
	}
	if (keys[SDL_SCANCODE_RIGHT]) {
		isButtonPressed = true;
		self->dir = DIR_RIGHT;
		self->position.x += 2;

		player_walk_frame_range(self, 4, 7);
		
	}
	if (keys[SDL_SCANCODE_LEFT]) {
		isButtonPressed = true;
		self->dir = DIR_LEFT;
		self->position.x -= 2;

		player_walk_frame_range(self, 12, 15);
		
	}

	if (!isButtonPressed) {
		if (self->dir == DIR_UP)self->frame = 8;
		if (self->dir == DIR_RIGHT)self->frame = 4;
		if (self->dir == DIR_DOWN)self->frame = 0;
		if (self->dir == DIR_LEFT)self->frame = 12;
		mf = 0;

	}
	//slog("mf Value is: %f", mf);
	//slog("Current frame is: %f", self->frame);
	mf += 0.1;


}

