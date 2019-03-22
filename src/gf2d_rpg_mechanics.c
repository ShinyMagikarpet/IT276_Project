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
		on_level_up(self);
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

void on_level_up(Entity *self) {
	int hpgrowth, strgrowth, defgrowth, agilgrowth;
	srand(time(0));
	hpgrowth	= roll_die(6, 4);
	//slog("HP Total: %i");
	strgrowth	= roll_die(4, 1);
	//slog("Str Total: %i");
	defgrowth	= roll_die(3, 4);
	//slog("Def Total: %i");
	agilgrowth	= roll_die(3, 1);
	//slog("Agi Total: %i");

	self->rpg.stats.hp_max += hpgrowth;
	self->rpg.stats.str += strgrowth;
	self->rpg.stats.def += defgrowth;
	self->rpg.stats.agil += agilgrowth;

}

int roll_die(int numsides, int numrolls) {
	int i, total;
	total = 0;
	for (i = 0; i < numrolls; i++) {
		total = (total + rand() %
			(numsides - 1 + 1)) + 1;
	}
	return total;

}

/*eol@eof*/