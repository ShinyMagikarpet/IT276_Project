#include "gf2d_rpg_mechanics.h"
#include "simple_logger.h"
#include <math.h>

//TODO: FIGURE OUT XP SYSTEM DISTRIBUTION

void check_if_level_up(Entity *self) {

	if (!self)return;
	
	int playerXP = self->rpg.xp;
	int playerLevel = self->rpg.level;


	while (playerXP >= xp_to_next_level(playerLevel)) {
		playerLevel++;
	}

	self->rpg.level = playerLevel;

}

void give_player_xp(Entity *player, Entity *enemy) {

	if (!player)return;
	if (!enemy)return;

	player->rpg.xp += enemy->rpg.xp;
	check_if_level_up(player);
}

int xp_to_next_level(int level) {

	level++;
	return round((4 * (pow(level, 3))) / 5);

}

int xp_remaining(int level, int xp) {
	
	return xp_to_next_level(level) - xp;

}

void gf2d_rpg_set_stats(Entity *self, int max_hp, int strength, int defence, int agility) {

	self->rpg.stats.hp_max = max_hp;
	self->rpg.stats.hp_current = self->rpg.stats.hp_max;
	self->rpg.stats.str = strength;
	self->rpg.stats.def = defence;
	self->rpg.stats.agil = agility;

}

/*eol@eof*/