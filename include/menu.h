#ifndef __MENU_H__
#define __MENU_H__
#include "gf2d_elements.h"
#include "gf2d_windows.h"
#include "gf2d_audio.h"
#include "items.h"

int Pause_Menu();

void Inventory();

void move_cursor_down();

void move_cursor_up();

void pause_menu_free();

Item *select_item(int cursorPos, List *inventory_list);

#endif // !__MENU_H__
