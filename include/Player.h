#ifndef _GF2D_Player_H__
#define _GF2D_Player_H__

#include "simple_logger.h"
#include "gf2d_entity.h"
#include "gf2d_rpg_mechanics.h"

#define PLAYER_VELOCITY 100

//Mask to prevent from player type shapes from hitting self
#define PLAYER_MASK_BIT (1<<31)

Entity *player_new(cpVect position, cpSpace *space);

Entity *player_get();

void player_set_position(cpVect position);

Entity *player_spawn(cpVect position, SJson *args, cpSpace *space);

int get_inventory_count();


#endif

