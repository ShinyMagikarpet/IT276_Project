#ifndef __FOX_H__
#define __FOX_H__

#include "simple_logger.h"
#include "gf2d_entity.h"

Entity *fox_new(cpVect position, cpSpace *space);

Entity *fox_spawn(cpVect position, SJson *args, cpSpace *space);


#endif 