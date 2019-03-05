#ifndef __SPAWN_H__
#define __SPAWN_H__

#include "gf2d_entity.h"
#include "simple_json.h"

typedef struct
{
	const char *name;
	Entity *(*spawn)(cpVect, SJson *args);
}Spawn;

void spawn_entity(const char *name, cpVect position, Uint32 id, SJson *args);

#endif
