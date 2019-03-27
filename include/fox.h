#ifndef __FOX_H__
#define __FOX_H__

#include "simple_logger.h"
#include "gf2d_entity.h"

/**
* @brief create new fox entity
* @param position on where to create
* @param space to place it in
*/
Entity *fox_new(cpVect position, cpSpace *space);

/**
* @brief spawn new fox
* @param position on where to create
* @param json arguments
* @param space to place it in
*/
Entity *fox_spawn(cpVect position, SJson *args, cpSpace *space);


#endif 