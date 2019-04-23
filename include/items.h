#ifndef __ITEMS_H__
#define __ITEMS_H__

#include "simple_logger.h"
#include <chipmunk_types.h>

#define IT_WEAPON 1
#define IT_ARMOR 2
#define IT_ACCESSORY 4
#define IT_COMSUMEABLES 8

#define MAX_ITEMS 256

typedef struct Item_s
{

	char	*name; /**<name of item*/
	int		inuse; /**<if item in use*/
	int		flag; /**<flag for item*/
	int		statvalue; /**<stat value for item*/
	int		index; /**<index for item in itemlist*/
	void	*data; /**<data pointer*/

	void(*equip)(struct Entity_S *ent, struct Item_s *item, int index); /**<calls equip function*/
	void(*use)(struct Entity_S *ent, struct Item_s *item, int index); /**<calls use function*/
	void(*drop)(struct Entity_S *ent, struct Item_s *item, int index); /**<calls drop function*/

} Item;

/**
* @brief gets item by index in item_list
* @param index The index of the item
* @return returns item
*/
Item *get_item_by_index(int index);

/**
* @brief gets item by name in item_list
* @param name Name of the item
* @return returns item
*/
Item *get_item_by_name(char *name);

/**
* @brief looks for empty spot in inventory and stores item value in inventory
* @param item Item to be stored
*/
void put_item_in_inventory(Item *item);

/**
* @brief returns number of items in item_list
* @return returns number of items
*/
int num_items();

/**
* @brief sorts the player's inventory when they drop or consume an item
* @param index The index of the item being removed
*/
void sort_inventory(int index);



extern Item item_list[]; /**item list for reference to everything in the program*/

//Gets the item's index in item_list
#define	ITEM_INDEX(x) ((x)-item_list)
#endif // !__ITEMS_H__
