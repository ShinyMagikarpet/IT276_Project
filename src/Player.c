#include "Player.h"
#include "simple_logger.h"

void player_update(Entity *self);

Entity *player_new(Vector2D position) {

	Entity *player = NULL;

	player = gf2d_entity_new();

	if (!player) {
		slog("failed to allocate new player");
		return NULL;
	}

	vector2d_copy(player->position, position);
	player->sprite = gf2d_sprite_load_all("images/ed210.png", 128, 128, 16);
	player->frame = 48;
	player->update = player_update;
	return player;

}

void player_update(Entity *self) {
	const Uint8 *keys;
	keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_UP])self->position.y -= 2;
	if (keys[SDL_SCANCODE_DOWN])self->position.y += 2;
	if (keys[SDL_SCANCODE_RIGHT])self->position.x += 2;
	if (keys[SDL_SCANCODE_LEFT])self->position.x -= 2;

}

