#ifndef __GF2D_ENTITY_H__
#define __GF2D_ENTITY_H__

#include "gf2d_vector.h"
#include "gf2d_text.h"
#include "gf2d_sprite.h"


typedef struct Entity_S {

	int _inuse;
	Uint64 id;
	Vector3D rotation;
	Vector2D position;
	Vector2D scale;
	Vector2D scalecenter;
	Vector2D flip;
	Sprite *sprite;
	float frame;


	void(*update)(struct Entity_S *self);
	void(*think)(struct Entity_S *self);
	void(*draw)(struct Entity_S *self);
	void(*touch)(struct Entity_S *self, struct Entity_S *other);
	void(*free)(struct Entity_S *self);
	int health;

} Entity;

void gf2d_entity_system_init(Uint32 maxEntities);

Entity *gf2d_entity_new();

void gf2d_entity_free(Entity *self);

void gf2d_entity_draw(Entity *self);

void gf2d_entity_draw_all();

void gf2d_entity_update(Entity *self);

#endif

