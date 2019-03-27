#ifndef _GF2D_Player_H__
#define _GF2D_Player_H__

#include "simple_logger.h"
#include "gf2d_entity.h"
#include "gf2d_rpg_mechanics.h"

//Player speed
#define PLAYER_VELOCITY 100

//Mask to prevent from player type shapes from hitting self
#define PLAYER_MASK_BIT (1<<31)

/**
 * @brief creates a new player instance
 * @param position The position to place the player
 * @param space the physics space to handle player physics
 */
Entity *player_new(cpVect position, cpSpace *space);

/**
 * @brief gets player entity
 */
Entity *player_get();

/**
 * @brief set the position of the player
 * @param position the postion to spawn the player
 */
void player_set_position(cpVect position);

/**
 * @brief spawn a player instance
 * @param position The position to place the player instance
 * @param args The information from json to provide more information for the player
 * @param space The physiscs space
 */
Entity *player_spawn(cpVect position, SJson *args, cpSpace *space);

/**
* @brief number of items in player's inventory
* @return returns integer of items
*/
int get_inventory_count();


#endif

