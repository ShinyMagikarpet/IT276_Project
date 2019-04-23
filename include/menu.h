#ifndef __MENU_H__
#define __MENU_H__
#include "items.h"
#include "gf2d_entity.h"

int Pause_Menu();

void Inventory();

void move_cursor_down();

void move_cursor_up();

void pause_menu_free();

Item *select_item(Entity *ent, int cursorPos);

#endif // !__MENU_H__
