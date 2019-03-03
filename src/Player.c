#include "Player.h"
#include "simple_logger.h"
#include "gf2d_input.h"
#include "camera.h"
#include "simple_json.h"


static Entity *player = NULL;
cpFloat moveX;
cpFloat moveY;

void player_think(Entity *self);
int player_touch(Entity *self);
void player_update(Entity *self);
void player_update_velocity(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt);
int cpTouch_player(cpBody *self, cpBody *other);

Entity *player_new(cpVect position, cpSpace *space) {

	player = gf2d_entity_new();

	if (!player) {
		slog("failed to allocate new player");
		return NULL;
	}

	//Chipmunk physics here
	player->cpbody = cpSpaceAddBody(space, cpBodyNew(5, INFINITY));
	player->cpbody->p = position;
	player->cpbody->userData = player;
	player->cpbody->velocity_func = player_update_velocity;
	player->cpshape = cpSpaceAddShape(space, cpCircleShapeNew(player->cpbody, 4, cpv(16, 16)));
	cpShapeSetCollisionType(player->cpshape, PLAYER_TYPE);
	
	//Debug for shape. otherwise useless
	player->shape = gf2d_shape_circle(32, 32, 16);

	//gf2d stuff
	player->inuse = 1;
	player->position = cpvector_to_gf2dvector(position);
	gf2d_line_cpy(player->name, "player");
	gf2d_actor_load(&player->actor, "actor/player.actor");
	gf2d_actor_set_action(&player->actor, "idle_down");
	vector2d_copy(player->scale, player->actor.al->scale);
	//player->actor.frame = 0;
	//vector2d_set(player->flip, 1, 0);
	player->dir = ED_Down;
	player->state = ES_Idle;


	//RPG stuff
	player->rpg.level = 1;
	player->rpg.xp = 0;
	//Stats
	player->rpg.stats.hp = 5;
	player->rpg.stats.str = 3;
	player->rpg.stats.def = 2;
	player->rpg.stats.agil = 3;

	player->iframes = 0;
	player->attack_rate = 10.0f / player->rpg.stats.agil;
	

	//player functions
	player->think = player_think;
	player->touch = player_touch;
	player->update = player_update;

	return player;
}

Entity *player_get() {
	return player;
}

Entity *player_spawn(cpVect position, SJson *args)
{
	if (player != NULL)
	{
		vector2d_copy(player->position, position);
		level_add_entity(player);
		return NULL;
	}
	return player_new(position, player->cpbody->space);
}

void player_set_position(Vector2D position) {

	if (!player) {
		slog("player not loaded");
		return;
	}

	vector2d_copy(player->position, position);
	vector2d_copy(player->body.position, position);

}

int cpTouch_player(cpBody *self, cpBody *other) {

	Entity *body1 = (Entity *)self->userData; //How to pull userdata Entity from body!!!
	
	if (other->userData == NULL)
		slog("TOUCHING GROUND");
	else {
		slog("TOUCHING MONSTER");
	}
	return 0;

}


int player_touch(Entity *self){

	//When player touches something
	//self->cpbody->v = cpvzero;
	if(player->cpbody->userData == NULL)
		slog("TOUCHING GROUND");
	else {
		slog("TOUCHING MONSTER");
	}
	return 0;

}

void player_update_velocity(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt) {

	body->v = cpv(moveX, moveY);

}

/**
* @brief Controls the player movement direction
* @param player entity
* @param direction that the player will move
*/


void player_think(Entity *self) {

	 moveX = 0;
	 moveY = 0;
	 cpSegmentQueryInfo hit = { 0 };
		 
	
	////Need to fix walking animation to easily
	////transition from idle to walking
	 if (gf2d_input_command_held("walk_up")) {
		 if (self->state != ES_Attack)
			 moveY -= PLAYER_VELOCITY;

		 if (self->state == ES_Idle) {
			 self->dir = ED_Up;
			 self->state = ES_Walk;
			 gf2d_actor_set_action(&self->actor, "walk_up");
		 }


	 }
	 else if (gf2d_input_command_get_state("walk_up") == IE_Release && self->state != ES_Attack) {
		 gf2d_actor_set_action(&self->actor, "idle_up");
		 self->cpbody->v.x = 0;
		 self->cpbody->v.y = 0;
		 self->state = ES_Idle;
	 }

	if (gf2d_input_command_held("walk_down")) {
		if (self->state != ES_Attack)
			moveY += PLAYER_VELOCITY;

		if (self->state == ES_Idle) {
			self->dir = ED_Down;
			self->state = ES_Walk;
			gf2d_actor_set_action(&self->actor, "walk_down");
		}


	}
	else if (gf2d_input_command_get_state("walk_down") == IE_Release && self->state != ES_Attack) {
		gf2d_actor_set_action(&self->actor, "idle_down");
		self->cpbody->v.x = 0;
		self->cpbody->v.y = 0;
		self->state = ES_Idle;
	}

	if (gf2d_input_command_held("walk_left")) {
		if (self->state != ES_Attack)
			moveX -= PLAYER_VELOCITY;

		if (self->state == ES_Idle) {
			self->dir = ED_Left;
			self->state = ES_Walk;
			gf2d_actor_set_action(&self->actor, "walk_left");
		}


	}
	else if (gf2d_input_command_get_state("walk_left") == IE_Release && self->state != ES_Attack) {
		gf2d_actor_set_action(&self->actor, "idle_left");
		self->cpbody->v.x = 0;
		self->cpbody->v.y = 0;
		self->state = ES_Idle;
	}

	if (gf2d_input_command_held("walk_right")) {
		if (self->state != ES_Attack)
			moveX += PLAYER_VELOCITY;

		if (self->state == ES_Idle) {
			self->dir = ED_Right;
			self->state = ES_Walk;
			gf2d_actor_set_action(&self->actor, "walk_right");
		}


	}
	else if (gf2d_input_command_get_state("walk_right") == IE_Release && self->state != ES_Attack) {
		gf2d_actor_set_action(&self->actor, "idle_right");
		self->cpbody->v.x = 0;
		self->cpbody->v.y = 0;
		self->state = ES_Idle;
	}

	if (gf2d_input_command_pressed("melee")) {

		int offsetRayX = 0, offsetRayY = 0;
		
		if (self->cooldown <= 0) {
			if (self->state == ES_Walk) {
				moveX = 0;
				moveY = 0;
				self->cpbody->v.x = 0;
				self->cpbody->v.y = 0;
			}

			switch (self->dir) {
			case ED_Down:
				gf2d_actor_set_action(&player->actor, "attack_down");
				offsetRayY = 100;
				break;
			case ED_Left:
				gf2d_actor_set_action(&player->actor, "attack_left");
				offsetRayX = -100;
				break;
			case ED_Right:
				gf2d_actor_set_action(&player->actor, "attack_right");
				offsetRayX = 100;
				break;
			case ED_Up:
				gf2d_actor_set_action(&player->actor, "attack_up");
				offsetRayY = -100;
				break;
			default:
				slog("WTF direction are you?!");
			}

			//Cast out a ray to check if we hit a monster
			if (cpSpaceSegmentQueryFirst(self->cpbody->space, self->cpbody->p, cpv(self->cpbody->p.x + offsetRayX, self->cpbody->p.y + offsetRayY), 2, CP_SHAPE_FILTER_ALL, &hit)) {

				if (hit.shape->type == MONSTER_TYPE)
					slog("WAAAAAAAAAAAAAAAh");

			}
			self->state = ES_Attack;
			self->cooldown = self->attack_rate;



		}


	}

	if (gf2d_input_command_held("walkup") && gf2d_input_command_held("walkdown")) {
		gf2d_actor_set_action(&self->actor, "idle_down");
		moveY = 0;
		self->state = ES_Idle;
	}

	if (self->actor.at == ART_END && self->state == ES_Attack) {
		switch (self->dir) {
		case ED_Down:
			gf2d_actor_set_action(&player->actor, "idle_down");
			break;
		case ED_Left:
			gf2d_actor_set_action(&player->actor, "idle_left");
			break;
		case ED_Right:
			gf2d_actor_set_action(&player->actor, "idle_right");
			break;
		case ED_Up:
			gf2d_actor_set_action(&player->actor, "idle_up");
			break;
		default:
			slog("WTF direction are you?!");
		}
		self->state = ES_Idle;
	}
	//Change velocity of player and copy it to position to update sprite???
	self->position = cpvector_to_gf2dvector(cpBodyGetPosition(self->cpbody));
	self->cpPos = cpBodyGetPosition(self->cpbody);
	gf2d_actor_next_frame(&self->actor);
}

void player_update(Entity *self) {
	
	

	


	if (self->cooldown > 0) {
		self->cooldown -= 0.025;
	}
		

	if (self->iframes > 0) {
		self->iframes -= 1;
		if (self->iframes == 0)
			slog("CAN BE ATTACKED AGAIN");
	}
}

