#ifndef __RED_SLIME_H__
#define __RED_SLIME_H__

#include "simple_logger.h"
#include "gf2d_entity.h"

Entity *red_slime_new(cpVect position, cpSpace *space);

Entity *red_slime_spawn(cpVect position, SJson *args, cpSpace *space);


#endif 