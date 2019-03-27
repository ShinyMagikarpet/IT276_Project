#ifndef __SPAWN_H__
#define __SPAWN_H__

#include "gf2d_entity.h"
#include "simple_json.h"

typedef struct
{
	const char *name;
	Entity *(*spawn)(cpVect, SJson *args, cpSpace *space);
}Spawn;

/**
 * @brief spawn entity
 * @param name The name of the entity to spawn
 * @param position The position to spawn the entity 
 * @param id The id number to give the entity
 * @param args The information that was given from json file
 * @param space The physics space
 */
void spawn_entity(const char *name, cpVect position, Uint32 id, SJson *args, cpSpace *space);

#endif
