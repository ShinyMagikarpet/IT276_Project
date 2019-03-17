#ifndef __HOUSE_H__
#define __HOUSE_H__
#include "gf2d_entity.h"
#include "simple_logger.h"

/**
* @brief spawn new house
* @param position on where to create
* @param json arguments
* @param space to place it in
*/
Entity *house_spawn(cpVect position, SJson *args, cpSpace *space);


#endif // !__HOUSE_H__
