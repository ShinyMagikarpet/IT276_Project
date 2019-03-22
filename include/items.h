#ifndef __ITEMS_H__
#define __ITEMS_H__

#include "simple_logger.h"

#define IT_WEAPON 1
#define IT_ARMOR 2
#define IT_ACCESSORY 4
#define IT_COMSUMEABLES 8


typedef struct Item_s
{

	char	*name;

	int		inuse;

	int		flag;

	int		statvalue;

	void	*data;

} Item;


extern Item item_list[];
#endif // !__ITEMS_H__
