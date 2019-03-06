#ifndef  __GF2D__RPG_MECHANICS__
#define __GF2D__RPG_MECHANICS__

#include "gf2d_entity.h"
/**
* @brief checks if player is ready to level up
* @param pointer to player
*/
void check_if_level_up(Entity *self);

/**
* @brief give player xp 
* @param pointer to player
* @param pointer to monster
*/
void give_player_xp(Entity *player, Entity *enemy);

/**
* @brief checks xp needed to reach next level
* @param current player level
* @see http://howtomakeanrpg.com/a/how-to-make-an-rpg-levels.html
*/
int xp_to_next_level(int level);

/**
* @brief checks xp needed to reach next level
* @param current player level
* @param current player xp
*/
int xp_remaining(int level, int xp);

/**
* @brief Sets up the stats of the entity
* @param Entity pointer
* @param Max health value
* @param Strength value
* @param Defence value
* @param Agility value
*/
void gf2d_rpg_set_stats(Entity *self, int max_hp, int strength, int defence, int agility);

#endif // ! __GF2D__RPG_MECHANICS__
