#include "items.h"
#include "gf2d_entity.h"
#include "Player.h"

Item *get_item_by_index(int index) {
	if (index == 0 || index >= num_items())
		return NULL;

	return &item_list[index];
}

Item *get_item_by_name(char *name) {
	int i;
	for (i = 1; i < num_items(); i++) {

		if (!item_list[i].name)continue;
		if (strcmp(item_list[i].name, name) == 0) {
			return &item_list[i];
		}
	}
	slog("No item found!");
	return NULL;
}

void *use_consumable(Entity *self, Item *item, int index) {

	//Never done bitwise stuff before, just want to know how it works
	if (item->flag &~ IT_COMSUMEABLES) {
		slog("Not a consumable");
		return;
	}
	if (!item)return;
	slog("consuming item");
	self->rpg.stats.hp_current += self->rpg.stats.hp_max * (float)(item->statvalue / 100.0);
	if (self->rpg.stats.hp_current > self->rpg.stats.hp_max)
		self->rpg.stats.hp_current = self->rpg.stats.hp_max;
	self->rpg.inventory[index] = NULL;
	
}

void *equip_weapon(Entity *self, Item *item, int index) {
	Item *replace_item;
	slog("This is a weapon");
	if (!item)return;
	replace_item = get_item_by_name(item->name);
	if (item->inuse == 1) {
		item->inuse = 0;
		memset(&self->rpg.equipped_weapon, 0, sizeof(Item));
		self->rpg.inventory[index] = replace_item;
		slog("unequipped item");
		return;
	}
	//Else we unequip the current item and change it to the new requested item
}

void *equip_armor(Entity *self, Item *item, int index) {
	slog("This is armor");
}

void *equip_accessory(Entity *self, Item *item, int index) {
	slog("This is an accessory");
}

void put_item_in_inventory(Item *item) {
	int i;
	Entity *player = player_get();

	for (i = 0; i < MAX_ITEMS; i++) {
		if (player->rpg.inventory[i] > 0)continue;
		player->rpg.inventory[i] = item;
		item->index = ITEM_INDEX(item);
		return;
	}

	slog("No more room in inventory");
	return;
}

//List of all items

Item item_list[] = {

	{NULL},

	//Weapons
	{
		"Wood Sword",
		0,
		IT_WEAPON,
		3,
		0,
		NULL,
		equip_weapon,
		NULL,
		NULL
	},
	{
		"Iron Sword",
		0,
		IT_WEAPON,
		6,
		0,
		NULL,
		equip_weapon,
		NULL,
		NULL
	},
	{
		"Steel Sword",
		0,
		IT_WEAPON,
		12,
		0,
		NULL,
		equip_weapon,
		NULL,
		NULL
	},
	//Aromor
	{
		"Leather Armor",
		0,
		IT_ARMOR,
		2,
		0,
		NULL,
		equip_armor,
		NULL,
		NULL
	},
	{
		"Iron Armor",
		0,
		IT_ARMOR,
		6,
		0,
		NULL,
		equip_armor,
		NULL,
		NULL
	},
	{
		"Steel Armor",
		0,
		IT_ARMOR,
		15,
		0,
		NULL,
		equip_armor,
		NULL,
		NULL
	},
	//Accessory
	{
		"Gold Pendant",
		0,
		IT_ACCESSORY,
		0,
		0,
		NULL,
		equip_accessory,
		NULL,
		NULL
	},
	{
		"Sapphire Pendant",
		0,
		IT_ACCESSORY,
		2,
		0,
		NULL,
		equip_accessory,
		NULL,
		NULL
	},
	{
		"Emerald Pendant",
		0,
		IT_ACCESSORY,
		4,
		0,
		NULL,
		equip_accessory,
		NULL,
		NULL
	},
	{
		"Ruby Pendant",
		0,
		IT_ACCESSORY,
		6,
		0,
		NULL,
		equip_accessory,
		NULL,
		NULL
	},
	//Comsumeables
	{
		"Sm. Potion",
		0,
		IT_COMSUMEABLES,
		20,
		0,
		NULL,
		NULL,
		use_consumable,
		NULL
	},
	{
		"Potion",
		0,
		IT_COMSUMEABLES,
		40,
		0,
		NULL,
		NULL,
		use_consumable,
		NULL
	},

	{NULL}

};

int num_items() {
	return (sizeof(item_list) / sizeof(item_list[0]) - 1);
}