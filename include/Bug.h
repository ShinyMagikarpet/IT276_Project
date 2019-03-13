#ifndef __BUG_H__
#define __BUG_H__

#include "simple_logger.h"
#include "gf2d_entity.h"

Entity *bug_new(cpVect position, cpSpace *space);

Entity *bug_spawn(cpVect position, SJson *args, cpSpace *space);


#endif 


