#include "red_slime.h"
#include "Player.h"
#include "gf2d_cpSpace.h"



void red_slime_think(Entity *self);
void red_slime_update(Entity *self);
int red_slime_damage(Entity *attacker, Entity *inflicted);

Entity *red_slime_new(cpVect position, cpSpace *space) {

	Entity *red_slime = NULL;

	red_slime = gf2d_entity_new();

	if (!red_slime) {
		slog("failed to allocate new red_slime");
		return;
	}

	//Chipmunk Physics
	red_slime->cpbody = cpBodyNew(100, INFINITY);
	cpBodySetPosition(red_slime->cpbody, position);
	red_slime->cpbody->userData = red_slime;
	red_slime->cpshape = cpCircleShapeNew(red_slime->cpbody, 15, cpv(12, 26));
	cpSpaceAddBody(space, red_slime->cpbody);
	cpSpaceAddShape(space, red_slime->cpshape);
	red_slime->cpshape->type = MONSTER_TYPE;
	red_slime->cpshape->u = 1.0f;
	red_slime->cpshape->e = 0;

	//gf2d info
	red_slime->spawnpos = position;
	vector2d_copy(red_slime->position, position);
	gf2d_actor_load(&red_slime->actor, "actor/red_slime.actor");
	gf2d_actor_set_action(&red_slime->actor, "idle");
	vector2d_copy(red_slime->scale, red_slime->actor.al->scale);
	red_slime->think = red_slime_think;
	red_slime->update = red_slime_update;
	red_slime->damage = red_slime_damage;
	red_slime->shape = gf2d_shape_circle(12, 26, 15);
	gf2d_line_cpy(red_slime->name, "red_slime");
	red_slime->flip.x = 1;
	red_slime->cooldown = 0;
	red_slime->attack_rate = 0;

	//RPG info
	gf2d_rpg_set_stats(red_slime, 3, 1, 1, 1);
	red_slime->rpg.xp = 20;

	return red_slime;


}

Entity *red_slime_spawn(cpVect position, SJson *args, cpSpace *space)
{
	if (!space) {
		slog("Space not there");
		return NULL;
	}
	return red_slime_new(position, space);
}

int red_slime_damage(Entity *attacker, Entity *inflicted) {
	int damage;
	if (!attacker)return 0;
	if (!inflicted)return 0;

	srand(time(0));

	damage = (attacker->rpg.stats.str + rand() % (5 - 3 + 1) + 3);
	return damage;
}

void red_slime_think(Entity *self) {
	float dirX, dirY;
	Entity *player = player_get();


	if (player) {

		//If near the player
		if (cpvnear(cpBodyGetPosition(self->cpbody), cpBodyGetPosition(player->cpbody), 150)) {
			
			//Get distance between player and red_slime
			dirX = player->cpbody->p.x - self->cpbody->p.x;
			dirY = player->cpbody->p.y - self->cpbody->p.y;

			//Normalize
			cpVect normal = cpvnormalize(cpv(dirX, dirY));

			self->state = ES_Walk;

			if (dirX > 0)
				self->flip.x = 1;
			else
				self->flip.x = 0;
			self->cpbody->v.x = normal.x * 50.0;
			self->cpbody->v.y = normal.y * 50.0;
			self->attack_rate = 1;

		}
		//Unused stat for this entity so using it to infinitely chase the player
		else if (self->attack_rate == 1) {
			//Get distance between player and red_slime
			dirX = player->cpbody->p.x - self->cpbody->p.x;
			dirY = player->cpbody->p.y - self->cpbody->p.y;

			//Normalize
			cpVect normal = cpvnormalize(cpv(dirX, dirY));

			self->state = ES_Walk;

			if (dirX > 0)
				self->flip.x = 1;
			else
				self->flip.x = 0;
			self->cpbody->v.x = normal.x * 50.0;
			self->cpbody->v.y = normal.y * 50.0;
			self->cooldown = 0;
		}
		else {
			//red_slime will patrol from right to left
			if (self->flip.x == 1) {
				self->cpbody->p.x += 1;
				self->cooldown -= 0.01;
				if (self->cooldown <= 0)
					self->flip.x = -1;

			}
			else {
				self->cpbody->p.x -= 1;
				self->cooldown += 0.01;
				if (self->cooldown >= 2)
					self->flip.x = 1;
			}

		}

	}
	self->position = cpvector_to_gf2dvector(cpBodyGetPosition(self->cpbody));
	gf2d_actor_next_frame(&self->actor);

}

void red_slime_update(Entity *self) {



}
