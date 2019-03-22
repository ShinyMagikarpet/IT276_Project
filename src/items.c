#include "items.h"
#include <stdlib.h>

//List of all items

Item item_list[] = {

	{NULL},

	//Weapons
	{
		"Wood Sword",
		0,
		IT_WEAPON,
		3,
		NULL
	},
	{
		"Iron Sword",
		0,
		IT_WEAPON,
		6,
		NULL
	},
	{
		"Steel Sword",
		0,
		IT_WEAPON,
		12,
		NULL
	},
	//Aromor
	{
		"Leather Armor",
		0,
		IT_ARMOR,
		2,
		NULL
	},
	{
		"Iron Armor",
		0,
		IT_ARMOR,
		6,
		NULL
	},
	{
		"Steel Armor",
		0,
		IT_ARMOR,
		15,
		NULL
	},
	//Accessory
	{
		"Sapphire Pendant",
		0,
		IT_ACCESSORY,
		2,
		NULL
	},
	{
		"Emerald Pendant",
		0,
		IT_ACCESSORY,
		4,
		NULL
	},
	{
		"Ruby Pendant",
		0,
		IT_ACCESSORY,
		6,
		NULL
	},
	//Comsumeables
	{
		"Sm. Potion",
		0,
		IT_COMSUMEABLES,
		20,
		NULL
	},
	{
		"Potion",
		0,
		IT_COMSUMEABLES,
		40,
		NULL
	},

	{NULL}

};