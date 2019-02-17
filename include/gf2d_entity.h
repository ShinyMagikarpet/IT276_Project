#ifndef __GF2D_ENTITY_H__
#define __GF2D_ENTITY_H__

#include "gf2d_vector.h"
#include "gf2d_text.h"
#include "gf2d_sprite.h"
#include "gf2d_actor.h"

typedef enum {
	ED_Up,
	ED_Down,
	ED_Right,
	ED_Left
}EntityDir;

typedef enum {

	ES_Idle,
	ES_Walk,
	ES_Attack

}EntityState;

typedef struct Entity_S {

	Uint8 inuse;
	Uint64 id;

	TextLine name;
	EntityState state;
	EntityDir dir;
	Action action;

	//Physics
	Vector2D velocity;
	Vector2D position;

	//Graphics
	Actor actor;
	Vector3D rotation;
	Vector2D scale;
	Vector2D scalecenter;
	Vector2D flip;
	Sprite *sprite;

	void(*update)(struct Entity_S *self);
	void(*think)(struct Entity_S *self);
	void(*draw)(struct Entity_S *self);
	void(*touch)(struct Entity_S *self, struct Entity_S *other);
	void(*free)(struct Entity_S *self);
	int health;

	//Game data
	void *data;

} Entity;

void gf2d_entity_system_init(Uint32 maxEntities);

Entity *gf2d_entity_new();

void gf2d_entity_free(Entity *self);

//void gf2d_entity_draw(Entity *self);

//void gf2d_entity_draw_all();

void gf2d_entity_think(Entity *self);

void gf2d_entity_think_all();

void gf2d_entity_update(Entity *self);

void gf2d_entity_update_all();

#endif

