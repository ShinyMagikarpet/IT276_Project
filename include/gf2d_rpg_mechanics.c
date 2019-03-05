#include "gf2d_rpg_mechanics.h"

void give_player_xp(Entity *player, Entity *enemy) {
	player->rpg.xp += enemy->rpg.xp;
}