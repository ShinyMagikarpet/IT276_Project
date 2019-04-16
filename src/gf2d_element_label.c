#include <stdlib.h>
#include <stdio.h>
#include "gf2d_element_label.h"
#include "simple_logger.h"
#include "Player.h"

void gf2d_element_label_draw(Element *element, Vector2D offset)
{
	LabelElement *label;
	Vector2D position;
	Vector2D size = { 0 };
	if (!element)return;
	label = (LabelElement*)element->data;
	if (!label)return;
	change_text_based_on_name(label, element->index); //Write text here if stat
	if (strlen(label->text) <= 0)return;
	size = gf2d_text_get_bounds(label->text, label->style);
	if (size.x < 0)
	{
		return;
	}
	// adjust position to top left
	vector2d_add(position, offset, element->bounds);
	switch (label->justify)
	{
	case LJ_Left:
		break;
	case LJ_Center:
		position.x += (element->bounds.w - size.x) / 2;
		break;
	case LJ_Right:
		position.x += (element->bounds.w - size.x);
		break;
	}
	switch (label->alignment)
	{
	case LA_Top:
		break;
	case LA_Middle:
		position.y += (element->bounds.h - size.y) / 2;
		break;
	case LA_Bottom:
		position.y += (element->bounds.h - size.y);
		break;
	}
	gf2d_text_draw_line(label->text, label->style, element->color, position);
}

List *gf2d_element_label_update(Element *element, Vector2D offset)
{
	return NULL;
}

void gf2d_element_label_free(Element *element)
{
	LabelElement *label;
	if (!element)return;
	label = (LabelElement*)element->data;
	if (label != NULL)
	{
		free(label);
	}
}

LabelElement *gf2d_element_label_new()
{
	LabelElement *label;
	label = (LabelElement *)malloc(sizeof(LabelElement));
	if (!label)
	{
		slog("failed to allocate memory for label");
		return NULL;
	}
	memset(label, 0, sizeof(LabelElement));
	return label;
}


LabelElement *gf2d_element_label_new_full(char *name, char *text, Color color, int style, int justify, int align)
{
	LabelElement *label;
	label = gf2d_element_label_new();
	if (!label)
	{
		return NULL;
	}
	gf2d_block_cpy(label->name, name);
	gf2d_block_cpy(label->text, text);
	label->bgcolor = color;
	label->style = style;
	label->justify = justify;
	label->alignment = align;
	return label;
}

void gf2d_element_make_label(Element *e, LabelElement *label)
{
	if (!e)return;
	e->data = label;
	e->type = ET_Label;
	e->draw = gf2d_element_label_draw;
	e->update = gf2d_element_label_update;
	e->free_data = gf2d_element_label_free;
}

void gf2d_element_label_set_text(Element *e, char *text)
{
	if (!e)return;
	if (e->type != ET_Label)return;
	LabelElement *label;
	label = (LabelElement *)e->data;
	if (!label)return;
	gf2d_block_cpy(label->text, text);
}

void gf2d_element_load_label_from_config(Element *e, SJson *json)
{
	SJson *value;
	Vector4D vector;
	Color color;
	const char *name, *buffer;
	int style = FT_Normal;
	int justify = LJ_Left;
	int align = LA_Top;
	if ((!e) || (!json))
	{
		slog("call missing parameters");
		return;
	}
	value = sj_object_get_value(json, "style");
	buffer = sj_get_string_value(value);
	if (buffer)
	{
		if (strcmp(buffer, "normal") == 0)
		{
			style = FT_Normal;
		}
		else if (strcmp(buffer, "small") == 0)
		{
			style = FT_Small;
		}
		else if (strcmp(buffer, "H1") == 0)
		{
			style = FT_H1;
		}
		else if (strcmp(buffer, "H2") == 0)
		{
			style = FT_H2;
		}
		else if (strcmp(buffer, "H3") == 0)
		{
			style = FT_H3;
		}
		else if (strcmp(buffer, "H4") == 0)
		{
			style = FT_H4;
		}
		else if (strcmp(buffer, "H5") == 0)
		{
			style = FT_H5;
		}
		else if (strcmp(buffer, "H6") == 0)
		{
			style = FT_H6;
		}
	}

	value = sj_object_get_value(json, "justify");
	buffer = sj_get_string_value(value);
	if (buffer)
	{
		if (strcmp(buffer, "left") == 0)
		{
			justify = LJ_Left;
		}
		else if (strcmp(buffer, "center") == 0)
		{
			justify = LJ_Center;
		}
		else if (strcmp(buffer, "right") == 0)
		{
			justify = LJ_Right;
		}
	}

	value = sj_object_get_value(json, "align");
	buffer = sj_get_string_value(value);
	if (buffer)
	{
		if (strcmp(buffer, "top") == 0)
		{
			align = LA_Top;
		}
		else if (strcmp(buffer, "middle") == 0)
		{
			align = LA_Middle;
		}
		else if (strcmp(buffer, "bottom") == 0)
		{
			align = LA_Bottom;
		}
	}
	value = sj_object_get_value(json, "color");
	vector4d_set(vector, 255, 255, 255, 255);
	sj_value_as_vector4d(value, &vector);
	color = gf2d_color_from_vector4(vector);

	value = sj_object_get_value(json, "name");
	name = sj_get_string_value(value);
	if (!name)
		name = "";

	value = sj_object_get_value(json, "text");
	buffer = sj_get_string_value(value);
	if (!buffer)
		buffer = "";
	
	//stat = sj_get_string_value(value);
	//TODO: FIX UI TO TAKE VARIABLE
	//sprintf(buffer, "%s: %i", stat, statvalue);
	
	gf2d_element_make_label(e, gf2d_element_label_new_full((char *)name, (char *)buffer, color, style, justify, align));

	
}

void change_text_based_on_name(LabelElement *label, int index) {
	int playerStat, player_equipment;
	char buffer[0x200];
	Entity *player = player_get();

	if (!label)return;
	if (!player)return;
	if (strcmp(label->name, "HEALTH") == 0) {
		playerStat = player->rpg.stats.hp_current;
		snprintf(buffer, sizeof(buffer), "HP: %i/%i", playerStat, player->rpg.stats.hp_max);
	}
	else if(strcmp(label->name, "LEVEL") == 0){
		playerStat = player->rpg.level;
		snprintf(buffer, sizeof(buffer), "%s: %i", label->name, playerStat);
	}
	else if(strcmp(label->name, "XP") == 0){
		playerStat = player->rpg.xp;
		snprintf(buffer, sizeof(buffer), "%s: %i", label->name, playerStat);
	}
	else if (strcmp(label->name, "STR") == 0) {
		playerStat = player->rpg.stats.str;
		player_equipment = player->rpg.equipped_weapon.statvalue;
		snprintf(buffer, sizeof(buffer), "%s: %i + %i", label->name, playerStat, player_equipment);
	}
	else if (strcmp(label->name, "DEF") == 0) {
		playerStat = player->rpg.stats.def;
		player_equipment = player->rpg.equipped_armor.statvalue;
		snprintf(buffer, sizeof(buffer), "%s: %i + %i", label->name, playerStat, player_equipment);
	}
	else if (strcmp(label->name, "AGI") == 0) {
		playerStat = player->rpg.stats.agil;
		player_equipment = player->rpg.equipped_accessory.statvalue;
		snprintf(buffer, sizeof(buffer), "%s: %i + %i", label->name, playerStat, player_equipment);
	}
	else if (strcmp(label->name, "Next") == 0) {
		playerStat = xp_remaining(player->rpg.level, player->rpg.xp);
		snprintf(buffer, sizeof(buffer), "%s: %i", label->name, playerStat);
	}
	else if (strcmp(label->name, "item") == 0) {
		snprintf(buffer, sizeof(buffer), "%s", label->text);
	}
	else {
		//This is a message
		return;
	}
	
	
	
	gf2d_block_cpy(label->text, buffer);

	//free(buffer);

}


/*eol@eof*/