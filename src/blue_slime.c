#include "blue_slime.h"
#include "Player.h"
#include "gf2d_cpSpace.h"



void blue_slime_think(Entity *self);
void blue_slime_update(Entity *self);
int blue_slime_damage(Entity *attacker, Entity *inflicted);

Entity *blue_slime_new(cpVect position, cpSpace *space) {

	Entity *blue_slime = NULL;

	blue_slime = gf2d_entity_new();

	if (!blue_slime) {
		slog("failed to allocate new blue_slime");
		return;
	}

	//Chipmunk Physics
	blue_slime->cpbody = cpBodyNew(100, INFINITY);
	cpBodySetPosition(blue_slime->cpbody, position);
	blue_slime->cpbody->userData = blue_slime;
	blue_slime->cpshape = cpCircleShapeNew(blue_slime->cpbody, 15, cpv(12, 26));
	cpSpaceAddBody(space, blue_slime->cpbody);
	cpSpaceAddShape(space, blue_slime->cpshape);
	blue_slime->cpshape->type = MONSTER_TYPE;
	blue_slime->cpshape->u = 1.0f;
	blue_slime->cpshape->e = 0;

	//gf2d info
	blue_slime->spawnpos = position;
	vector2d_copy(blue_slime->position, position);
	gf2d_actor_load(&blue_slime->actor, "actor/blue_slime.actor");
	gf2d_actor_set_action(&blue_slime->actor, "idle");
	vector2d_copy(blue_slime->scale, blue_slime->actor.al->scale);
	blue_slime->think = blue_slime_think;
	blue_slime->update = blue_slime_update;
	blue_slime->damage = blue_slime_damage;
	blue_slime->shape = gf2d_shape_circle(12, 26, 15);
	gf2d_line_cpy(blue_slime->name, "blue_slime");
	blue_slime->flip.x = 1;
	blue_slime->cooldown = 0;

	//RPG info
	gf2d_rpg_set_stats(blue_slime, 3, 1, 1, 1);
	blue_slime->rpg.xp = 20;

	return blue_slime;


}

Entity *blue_slime_spawn(cpVect position, SJson *args, cpSpace *space)
{
	if (!space) {
		slog("Space not there");
		return NULL;
	}
	return blue_slime_new(position, space);
}

int blue_slime_damage(Entity *attacker, Entity *inflicted) {
	int damage;
	if (!attacker)return 0;
	if (!inflicted)return 0;

	srand(time(0));

	damage = (attacker->rpg.stats.str + rand() % (5 - 3 + 1) + 3);
	return damage;
}

void blue_slime_think(Entity *self) {
	float dirX, dirY;
	Entity *player = player_get();
	cpBool returned = cpTrue;


	if (player) {

		if (cpvnear(cpBodyGetPosition(self->cpbody), cpBodyGetPosition(player->cpbody), 150)) {

			//Get distance between player and blue_slime
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
			returned = cpFalse;

		}
		//Return to spawn position
		else if (!returned && self->cpbody->p.x != self->spawnpos.x || self->cpbody->p.y != self->spawnpos.y) {

			self->state = ES_Walk;

			//Get distance between blue_slime current position to spawn position
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

			//Make sure the blue_slime is close enough to spawn point 
			if (cpvnear(self->cpbody->p, self->spawnpos, 1)) {
				self->cpbody->p = self->spawnpos;
				self->cpbody->v = cpvzero;
				returned = cpTrue;
			}

		}
		else {
			//blue_slime will patrol from right to left
			if (self->flip.x == 1) {
				self->cpbody->p.x += 1;
				self->cooldown -= 0.01;
				if(self->cooldown <= 0)
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

void blue_slime_update(Entity *self) {



}
