#ifndef _GF2D_Player_H__
#define _GF2D_Player_H__

#include "simple_logger.h"
#include "gf2d_entity.h"

#define PLAYER_VELOCITY 100


Entity *player_new(cpVect position);

Entity *player_get();

void player_set_position(Vector2D position);

Entity *player_spawn(Vector2D position, SJson *args);


#endif

