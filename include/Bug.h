#ifndef __BUG_H__
#define __BUG_H__

#include "simple_logger.h"
#include "gf2d_entity.h"
/**
* @brief create new bug entity
* @param position on where to create
* @param space to place it in
*/
Entity *bug_new(cpVect position, cpSpace *space);

/**
* @brief spawn new bug
* @param position on where to create
* @param json arguments
* @param space to place it in
*/
Entity *bug_spawn(cpVect position, SJson *args, cpSpace *space);


#endif 


