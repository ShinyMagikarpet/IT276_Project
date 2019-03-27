#ifndef __BLUE_SLIME_H__
#define __BLUE_SLIME_H__

#include "simple_logger.h"
#include "gf2d_entity.h"

/**
* @brief create new blue slime entity
* @param position on where to create
* @param space to place it in
*/
Entity *blue_slime_new(cpVect position, cpSpace *space);

/**
* @brief spawn new blue slime
* @param position on where to create
* @param json arguments
* @param space to place it in
*/
Entity *blue_slime_spawn(cpVect position, SJson *args, cpSpace *space);


#endif 