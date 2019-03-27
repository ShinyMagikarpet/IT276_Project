#ifndef __BLUE_SLIME_H__
#define __BLUE_SLIME_H__

#include "simple_logger.h"
#include "gf2d_entity.h"

Entity *blue_slime_new(cpVect position, cpSpace *space);

Entity *blue_slime_spawn(cpVect position, SJson *args, cpSpace *space);


#endif 