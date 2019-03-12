#ifndef __BUG_HIVE_H__
#define __BUG_HIVE_H__

#include "gf2d_entity.h"
#include "simple_logger.h"

Entity *bug_hive_new(cpVect position, cpSpace *space);

Entity *bug_hive_spawn(cpVect position, SJson *args, cpSpace *space);


#endif // !__BUG_HIVE_H__
