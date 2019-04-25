#include "Player.h"
#include "simple_logger.h"
#include "gf2d_input.h"
#include "camera.h"
#include "simple_json.h"
#include "gf2d_entity_common.h"
#include <SDL_mixer.h>
#include "gf2d_transition.h"
#include "gf2d_cpSpace.h"
#include "particle_effects.h"


static Entity *player = NULL;
static RpgElements rpg; /**< used to transfer player data when loaded */
static cpFloat moveX;
static cpFloat moveY;
static cpBool musicpause = cpFalse;

//Filter to prevent hitscan from player hitting self
cpShapeFilter PLAYER_FILTER = { CP_NO_GROUP, PLAYER_MASK_BIT, PLAYER_MASK_BIT };
cpShapeFilter NOT_PLAYER_FILTER = { CP_NO_GROUP, ~PLAYER_MASK_BIT, ~PLAYER_MASK_BIT };

void player_think(Entity *self);
void player_update(Entity *self);
void player_update_velocity(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt);
int cpTouch_player(cpBody *self, cpBody *other);
int player_damage(Entity *attacker, Entity *inflicted);
void player_free(Entity *self);

Entity *player_new(cpVect position, cpSpace *space) {

	Item *item;

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
	player->cpshape = cpSpaceAddShape(space, cpCircleShapeNew(player->cpbody, 15, cpv(32, 32)));
	player->cpshape->userData = player;
	cpShapeSetFilter(player->cpshape, NOT_PLAYER_FILTER);
	cpShapeSetCollisionType(player->cpshape, PLAYER_TYPE);
	
	//Debug for shape. otherwise useless
	player->shape = gf2d_shape_circle(32, 32, 15);
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
	//Check if this is first time being initialized
	if (rpg.level <= 0) {
		player->rpg.level = 1;
		player->rpg.xp = 0;
		gf2d_rpg_set_stats(player, 10, 1, 1, 3);

		//Weapon
		item = get_item_by_name("Wood Sword");
		item->inuse = 0;
		player->rpg.equipped_weapon = *item;
		player->rpg.equipped_weapon.inuse = 1;
		slog("Weapon list usagee: %i", item_list[1].inuse);
		item = &player->rpg.equipped_weapon;
		put_item_in_inventory(item);

		//Armor
		item = get_item_by_name("Leather Armor");
		player->rpg.equipped_armor = *item;
		player->rpg.equipped_armor.inuse = 1;
		slog("Armor equipped: %s", player->rpg.equipped_armor.name);
		item = &player->rpg.equipped_armor;
		put_item_in_inventory(item);

		//Accessory
		item = get_item_by_name("Gold Pendant");
		player->rpg.equipped_accessory = *item;
		player->rpg.equipped_accessory.inuse = 1;
		slog("Accessory equipped: %s", player->rpg.equipped_accessory.name);
		item = &player->rpg.equipped_accessory;
		put_item_in_inventory(item);

		//Potion
		item = get_item_by_name("Potion");
		item->inuse = 0;
		put_item_in_inventory(item);
		
		//Potion
		item = get_item_by_name("Potion");
		item->inuse = 0;
		put_item_in_inventory(item);

		//Second Weapon for testing
		item = get_item_by_name("Iron Sword");
		put_item_in_inventory(item);
	}
	else {
		player->rpg = rpg;
	}

	player->iframes = 0;
	player->attack_rate = 1.0f;
	player->sound[0] = gf2d_sound_load("sounds/sword_swing.mp3", 1, -1);
	

	//player functions
	player->think = player_think;
	player->update = player_update;
	player->damage = player_damage;
	player->free = player_free;

	return player;
}

Entity *player_get() {
	return player;
}

void player_free(Entity *self) {

	rpg = player->rpg;
	gf2d_entity_free(self);
	//This variable still had memory address and was sending false positives throughout the code
	//This makes sure that everything about player is nulled out!
	player = NULL;
}

Entity *player_spawn(cpVect position, SJson *args, cpSpace *space)
{
	if (player != NULL)
	{
		vector2d_copy(player->position, position);
		return NULL;
	}
	if (!space) {
		slog("Space not there");
		return NULL;
	}
	return player_new(position, space);
}

void player_set_position(cpVect position) {

	if (!player) {
		slog("player not loaded");
		return;
	}
	/*vector2d_copy(player->position, position);
	vector2d_copy(player->body.position, position);*/
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

int player_damage(Entity *attacker, Entity *inflicted) {
	int damage;
	if (!attacker)return 0;
	if (!inflicted)return 0;

	
	damage = (attacker->rpg.stats.str + attacker->rpg.equipped_weapon.statvalue);
	return damage;
}

void player_update_velocity(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt) {
	cpVect normal = cpvnormalize(cpv(moveX, moveY));
	body->v = cpvmult(normal, PLAYER_VELOCITY);
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
		 
	 if (!self)return;
	////Need to fix walking animation to easily
	////transition from idle to walking
	 if (gf2d_input_command_held("move_up")) {
		 if (self->state != ES_Attack)
			 moveY -= PLAYER_VELOCITY;

		 if (self->state == ES_Idle) {
			 self->dir = ED_Up;
			 self->state = ES_Walk;
			 gf2d_actor_set_action(&self->actor, "move_up");
		 }


	 }
	 else if (gf2d_input_command_get_state("move_up") == IE_Release && self->state != ES_Attack) {
		 gf2d_actor_set_action(&self->actor, "idle_up");
		 self->cpbody->v.x = 0;
		 self->cpbody->v.y = 0;
		 self->state = ES_Idle;
	 }

	if (gf2d_input_command_held("move_down")) {
		if (self->state != ES_Attack)
			moveY += PLAYER_VELOCITY;

		if (self->state == ES_Idle) {
			self->dir = ED_Down;
			self->state = ES_Walk;
			gf2d_actor_set_action(&self->actor, "move_down");
		}


	}
	else if (gf2d_input_command_get_state("move_down") == IE_Release && self->state != ES_Attack) {
		gf2d_actor_set_action(&self->actor, "idle_down");
		self->cpbody->v.x = 0;
		self->cpbody->v.y = 0;
		self->state = ES_Idle;
	}

	if (gf2d_input_command_held("move_left")) {
		if (self->state != ES_Attack)
			moveX -= PLAYER_VELOCITY;

		if (self->state == ES_Idle) {
			self->dir = ED_Left;
			self->state = ES_Walk;
			gf2d_actor_set_action(&self->actor, "move_left");
		}


	}
	else if (gf2d_input_command_get_state("move_left") == IE_Release && self->state != ES_Attack) {
		gf2d_actor_set_action(&self->actor, "idle_left");
		self->cpbody->v.x = 0;
		self->cpbody->v.y = 0;
		self->state = ES_Idle;
	}

	if (gf2d_input_command_held("move_right")) {
		if (self->state != ES_Attack)
			moveX += PLAYER_VELOCITY;

		if (self->state == ES_Idle) {
			self->dir = ED_Right;
			self->state = ES_Walk;
			gf2d_actor_set_action(&self->actor, "move_right");
		}


	}
	else if (gf2d_input_command_get_state("move_right") == IE_Release && self->state != ES_Attack) {
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
				offsetRayY = 35;
				break;
			case ED_Left:
				gf2d_actor_set_action(&player->actor, "attack_left");
				offsetRayX = -35;
				break;
			case ED_Right:
				gf2d_actor_set_action(&player->actor, "attack_right");
				offsetRayX = 35;
				break;
			case ED_Up:
				gf2d_actor_set_action(&player->actor, "attack_up");
				offsetRayY = -35;
				break;
			default:
				slog("WTF direction are you?!");
			}

			//Cast out a ray to check if we hit a monster
			if (cpSpaceSegmentQueryFirst(self->cpbody->space, cpv(self->cpbody->p.x + 32, self->cpbody->p.y + 32), cpv(self->cpbody->p.x + 32 + offsetRayX, self->cpbody->p.y + 32 + offsetRayY), 10, PLAYER_FILTER, &hit)) {

				if (hit.shape->type == MONSTER_TYPE) {
					Entity *inflicted = hit.shape->body->userData;
					if (inflicted) {
						slog("Hit the monster!");
						inflicted->cpbody->p = cpvadd(inflicted->cpbody->p, cpv(50 * -hit.normal.x, 50 * -hit.normal.y));
						inflicted->rpg.stats.hp_current -= self->damage(self, inflicted);
						//if inclicted dies, reward player
						if (inflicted->rpg.stats.hp_current <= 0) {

							give_player_xp(self, inflicted);
							
							//Makes sure that the process is handled after space step to avoid body/shape removal errors
							player_touch_monster_postsolve(NULL, self->cpbody->space, inflicted);
						}
						
							
					}
					
				}
				//if chest
				else if (hit.shape->type == INTERACTABLE_TYPE) {
					Entity *chest = hit.shape->body->userData;
					if (chest) {
						//check if the chest has a legal item as 0 is null
						if (chest->rpg.selected_item != 0) {
							put_item_in_inventory(get_item_by_index(chest->rpg.selected_item));
							//take away chest contents
							chest->rpg.selected_item = 0;
						}
						
					}
					
				}
					

			}
			gf2d_sound_play(self->sound[0], 0, 1, -1, -1);
			self->state = ES_Attack;
			self->cooldown = self->attack_rate;
			particle_spray(cpvector_to_gf2dvector(self->cpbody->p), vector2d(0, -2), gf2d_color8(255, 0, 0, 200), 100);

		}


	}

	if (gf2d_input_command_held("moveup") && gf2d_input_command_held("movedown")) {
		gf2d_actor_set_action(&self->actor, "idle_down");
		moveY = 0;
		self->state = ES_Idle;
	}

	//Mute music
	if (gf2d_input_command_pressed("mute")) {
		if (musicpause == 0) {
			Mix_PauseMusic();
			musicpause = cpTrue;
		}
		else {
			Mix_ResumeMusic();
			musicpause = cpFalse;
		}
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

	//DEBUG
	const Uint8 * keys;
	keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_1]) {

		int i;
		for (i = 0; i < MAX_ITEMS; i++) {
			if(player->rpg.inventory[i] != 0)
				slog("inventory slot %i = %i", i, player->rpg.inventory[i]);
			else {
				break;
			}
			
		}
		slog("inventory count: %i", get_inventory_count());
		
	}
	if (keys[SDL_SCANCODE_2]) {
		Item *item;

		item = get_item_by_index(12);
		put_item_in_inventory(item);

		item = get_item_by_index(7);
		put_item_in_inventory(item);



	}
	if (keys[SDL_SCANCODE_3]) {

		//slog("Item retrieved: %s", item->name);
		Item *item;
		//slog("Item is currently in use: %i", player->rpg.inventory[0]->inuse);
		//item = player->rpg.inventory[3];
		//slog("Item is currently in use: %i", player->rpg.inventory[3]->inuse);
		for (int i = 0; i < MAX_ITEMS; i++) {
			item = player->rpg.inventory[i];
			if (!item)break;
			slog("%s usage is: %i", player->rpg.inventory[i]->name, player->rpg.inventory[i]->inuse);
		}

	}

	self->position = cpvector_to_gf2dvector(cpBodyGetPosition(self->cpbody));
	gf2d_actor_next_frame(&self->actor);

	
}

void player_update(Entity *self) {

	if (!self)return;
	if (self->cooldown > 0) {
		self->cooldown -= 0.015;
	}
		

	if (self->iframes > 0.0) {
		self->iframes -= 0.01;
		if (self->iframes == 0.0)
			slog("CAN BE ATTACKED AGAIN");
	}
}

int get_inventory_count() {
	int i, count;
	count = 0;
	for (i = 0; i < MAX_ITEMS; i++) {
		if (player->rpg.inventory[i] != 0) {
			count++;
			continue;
		}
		return count;
	}
}

