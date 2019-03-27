#ifndef __CHEST_H__
#define __CHEST_H__

#include "gf2d_entity.h"
#include "simple_logger.h"

/**
* @brief create new chest entity
* @param position on where to create
* @param space to place it in
*/
Entity *chest_new(cpVect position, cpSpace *space);

/**
* @brief spawn new chest
* @param position on where to create
* @param json arguments
* @param space to place it in
*/
Entity *chest_spawn(cpVect position, SJson *args, cpSpace *space);


#endif // !__BUG_HIVE_H__
