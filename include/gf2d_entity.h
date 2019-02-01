#ifndef __GF2D_ENTITY_H__
#define __GF2D_ENTITY_H__

#include "gf2d_vector.h"
#include "gf2d_text.h"
#include "gf2d_sprite.h"



typedef struct Entity_S {

	int _inuse;
	Vector3D rotation;
	Vector2D position;
	Vector2D scale;
	Sprite *sprite;
	

	void (update*)(struct Entity_S *self);
	void (think*)(struct Entity_S *self);
	void (touch*)(struct Entity_S *self, struct Entity_S *other);
	int health;

} Entity;

#endif
