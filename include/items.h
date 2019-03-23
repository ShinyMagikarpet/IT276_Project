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

	char	*name;
	int		inuse;
	int		flag;
	int		statvalue;
	int		index;
	void	*data;

	void(*equip)(struct Entity_S *ent, struct Item_s *item);
	void(*use)(struct Entity_S *ent, struct Item_s *item);
	void(*drop)(struct Entity_S *ent, struct Item_s *item);

} Item;

Item *get_item_by_index(int index);

Item *get_item_by_name(char *name);

void put_item_in_inventory(Item *item);

int num_items();



extern Item item_list[];

#define	ITEM_INDEX(x) ((x)-item_list)
#endif // !__ITEMS_H__
