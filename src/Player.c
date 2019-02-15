#include "Player.h"
#include "simple_logger.h"
#include "gf2d_input.h"


static Entity *player = NULL;

void player_think(Entity *self);
void player_update(Entity *self);

Entity *player_new(Vector2D position) {

	//Entity *self;
	player = gf2d_entity_new();

	if (!player) {
		slog("failed to allocate new player");
		return NULL;
	}

	gf2d_line_cpy(player->name, "player");
	gf2d_actor_load(&player->actor, "actor/player.actor");
	gf2d_actor_set_action(&player->actor, "idle_down");
	vector2d_copy(player->position, position);
	//player->sprite = gf2d_sprite_load_all("images/character.png", 16, 32, 4);
	//vector2d_copy(player->scale, player->actor.al->scale);
	player->scale.x = 2;
	player->scale.y = 2;
	//player->actor.frame = 0;
	//vector2d_set(player->flip, 1, 0);
	player->think = player_think;
	player->update = player_update;
	player->state = ES_Idle;
	return player;
}

Entity *player_get() {
	return player;
}

void player_think(Entity *self) {

}

void player_update(Entity *self) {

	const Uint8 *keys;

	keys = SDL_GetKeyboardState(NULL);
	
	//Need to fix walking animation to easily
	//transition from idle to walking
	/*if (keys[SDL_SCANCODE_UP]) {
		self->position.y -= 2;

		if (self->state != ES_Walk)
			gf2d_actor_set_action(&self->actor, "walk_up");
		else
			gf2d_actor_next_frame(&self->actor);

		self->state = ES_Walk;
		
	}*/
		
	if (gf2d_input_command_held("walkup")) {
		self->position.y -= 2;

		if (self->state == ES_Idle) {
			self->state = ES_Walk;
			gf2d_actor_set_action(&self->actor, "walk_up");
		}
		else
			gf2d_actor_next_frame(&self->actor);
		
	}
	else if (gf2d_input_command_get_state("walkup") == IE_Release) {
		gf2d_actor_set_action(&self->actor, "idle_up");
		self->state = ES_Idle;
	}

	if (gf2d_input_command_held("walkdown")) {
		self->position.y += 2;

		if (self->state == ES_Idle) {
			self->state = ES_Walk;
			gf2d_actor_set_action(&self->actor, "walk_down");
		}
		else
			gf2d_actor_next_frame(&self->actor);

	}
	else if (gf2d_input_command_get_state("walkdown") == IE_Release) {
		gf2d_actor_set_action(&self->actor, "idle_down");
		self->state = ES_Idle;
	}

	/*if (keys[SDL_SCANCODE_DOWN]) {
		self->position.y += 2;

		if (self->state != ES_Walk)
			gf2d_actor_set_action(&self->actor, "walk_down");
		else
			gf2d_actor_next_frame(&self->actor);

		self->state = ES_Walk;
		
	}*/

	if (gf2d_input_command_held("walkleft")) {
		self->position.x -= 2;

		if (self->state == ES_Idle) {
			self->state = ES_Walk;
			gf2d_actor_set_action(&self->actor, "walk_left");
		}
		else
			gf2d_actor_next_frame(&self->actor);

	}
	else if (gf2d_input_command_get_state("walkleft") == IE_Release) {
		gf2d_actor_set_action(&self->actor, "idle_left");
		self->state = ES_Idle;
	}

	if (gf2d_input_command_held("walkright")) {
		self->position.x += 2;

		if (self->state == ES_Idle) {
			self->state = ES_Walk;
			gf2d_actor_set_action(&self->actor, "walk_right");
		}
		else
			gf2d_actor_next_frame(&self->actor);

	}
	else if (gf2d_input_command_get_state("walkright") == IE_Release) {
		gf2d_actor_set_action(&self->actor, "idle_right");
		self->state = ES_Idle;
	}

}

