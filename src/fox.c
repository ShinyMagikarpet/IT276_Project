#include "fox.h"
#include "Player.h"
#include "gf2d_cpSpace.h"



void fox_think(Entity *self);
void fox_update(Entity *self);
int fox_damage(Entity *attacker, Entity *inflicted);

Entity *fox_new(cpVect position, cpSpace *space) {

	Entity *fox = NULL;

	fox = gf2d_entity_new();

	if (!fox) {
		slog("failed to allocate new fox");
		return;
	}

	//Chipmunk Physics
	fox->cpbody = cpBodyNew(100, INFINITY);
	cpBodySetPosition(fox->cpbody, position);
	fox->cpbody->userData = fox;
	fox->cpshape = cpCircleShapeNew(fox->cpbody, 15, cpv(24, 24));
	cpSpaceAddBody(space, fox->cpbody);
	cpSpaceAddShape(space, fox->cpshape);
	fox->cpshape->type = MONSTER_TYPE;
	fox->cpshape->u = 1.0f;
	fox->cpshape->e = 0;

	//gf2d info
	fox->spawnpos = position;
	vector2d_copy(fox->position, position);
	gf2d_actor_load(&fox->actor, "actor/fox.actor");
	gf2d_actor_set_action(&fox->actor, "idle");
	vector2d_copy(fox->scale, fox->actor.al->scale);
	fox->think = fox_think;
	fox->update = fox_update;
	fox->damage = fox_damage;
	fox->shape = gf2d_shape_circle(24, 24, 15);
	gf2d_line_cpy(fox->name, "fox");
	fox->cooldown = 0;

	//RPG info
	gf2d_rpg_set_stats(fox, 3, 1, 1, 1);
	fox->rpg.xp = 20;

	return fox;


}

Entity *fox_spawn(cpVect position, SJson *args, cpSpace *space)
{
	if (!space) {
		slog("Space not there");
		return NULL;
	}
	return fox_new(position, space);
}

int fox_damage(Entity *attacker, Entity *inflicted) {
	int damage;
	if (!attacker)return 0;
	if (!inflicted)return 0;

	srand(time(0));

	damage = (attacker->rpg.stats.str + rand() % (5 - 3 + 1) + 3);
	return damage;
}

void fox_think(Entity *self) {
	float dirX, dirY;
	Entity *player = player_get();


	
		
	//fox will patrol from right to left
	if (self->flip.x == 0) {
		self->cpbody->p.x += 1;
		self->cooldown -= 0.01;
		if (self->cooldown <= 0) {
			self->flip.x = 1;
		}
			

	}
	else {
		self->cpbody->p.x -= 1;
		self->cooldown += 0.01;
		if (self->cooldown >= 2) {
			self->flip.x = 0;
		}
			
	}
	
	

}

void fox_update(Entity *self) {

	self->position = cpvector_to_gf2dvector(cpBodyGetPosition(self->cpbody));
	gf2d_actor_next_frame(&self->actor);

}