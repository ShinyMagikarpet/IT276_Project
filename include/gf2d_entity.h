#ifndef __GF2D_ENTITY_H__
#define __GF2D_ENTITY_H__
#include <chipmunk.h>
#include <chipmunk_structs.h>
#include "gf2d_vector.h"
#include "gf2d_text.h"
#include "gf2d_sprite.h"
#include "gf2d_actor.h"
#include "gf2d_collision.h"
#include "simple_json.h"
#include "gf2d_cpSpace.h"
#include "camera.h"


typedef enum {
	ED_Up,		/**<entity is facing up*/
	ED_Down,	/**<entity is facing down*/
	ED_Right,	/**<entity is facing right*/
	ED_Left		/**<entity is facing left*/
}EntityDir;

typedef enum {

	ES_Idle,	/**<entity is currently idle*/
	ES_Walk,	/**<entity is currently walking*/
	ES_Attack	/**<entity is currently attacking*/

}EntityState;

typedef struct {
	int hp_max;		/**<entity's max health*/
	int hp_current;	/**<entity's health*/
	int str;		/**<entity's strength*/
	int def;		/**<entity's defence*/
	int agil;		/**<entity's agility*/
}Stats;

typedef struct{

	int xp; /**<xp of entity*/
	int level; /**<level of entity*/
	Stats stats; /**<stat struct for entity*/

}RpgElements;

typedef struct
{
	RpgElements rpg;
	//Placeholder: Inventory
	//Placeholder: Currently Equipped

} Player_Persistant; /**<Keep info for player between levels*/

typedef struct Entity_S {

	Player_Persistant pers;

	Uint8 inuse;
	Uint32 id;

	TextLine name;
	EntityState state;
	EntityDir dir;
	Action action;

	//Physics
	cpShape *cpshape;
	cpBody *cpbody;

	cpVect spawnpos; /**<Spawn position of entity*/
	Shape shape;
	Body body;
	Vector2D velocity;
	Vector2D position;

	//Graphics
	Actor actor;
	Vector3D rotation;
	Vector2D scale;
	Vector2D scalecenter;
	Vector2D flip;
	Sprite *sprite;

	void(*update)(struct Entity_S *self); /**<update entity per frame*/
	void(*think)(struct Entity_S *self); /**<think before update*/
	void(*draw)(struct Entity_S *self); /**<render entity*/
	int(*touch)(struct Entity_S *self); /**<currently not using*/
	int(*cpTouch)(cpBody *self, cpBody *other); /**<called when player collides something*/
	int(*damage)(struct Entity_S *attacker, int amount, struct Entity_S *inflicted);/**<when this entity takes damage*/
	void(*free)(struct Entity_S *self); /**<free entity from game*/

	//RPG stuff and stat stuff
	RpgElements rpg;

	float attack_rate; /**<rate of which an entity can attack*/
	float cooldown; /**<cooldown for attacks*/
	int iframes; /**<time of invicibility frames*/

	//Game data
	void *data; /**<data pointer*/

} Entity;

void gf2d_entity_system_init(Uint32 maxEntities);

Entity *gf2d_entity_new();

void gf2d_entity_free(Entity *self);

/**
* @brief free all entities
*/
void gf2d_entity_free_all();

void gf2d_entity_draw(Entity *self);

void gf2d_entity_draw_all();

/**
* @brief Draws the shape of an entity
* @param The entity to be drawn
*/
void gf2d_entity_draw_shape(Entity *self);

/**
* @brief Draws the shape of all entities
* @param The entity to be drawn
*/
void gf2d_entity_draw_shape_all();

void gf2d_entity_think(Entity *self);

void gf2d_entity_think_all();

void gf2d_entity_update(Entity *self);

void gf2d_entity_update_all();

void gf2d_entity_pre_sync_all();

void gf2d_entity_post_sync_all();

Entity *gf2d_entity_get_by_id(Uint32 id);

Entity *gf2d_entity_get_by_name_id(const char *name, Uint32 id);

Entity *gf2d_entity_iterate(Entity *start);

#endif

