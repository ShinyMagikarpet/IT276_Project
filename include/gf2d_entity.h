#ifndef __GF2D_ENTITY_H__
#define __GF2D_ENTITY_H__
#include <chipmunk.h>
#include <chipmunk_structs.h>
#include "gf2d_vector.h"
#include "gf2d_text.h"
#include "gf2d_sprite.h"
#include "gf2d_actor.h"
#include "simple_json.h"
#include "items.h"
#include "gf2d_shape.h"
#include "gf2d_audio.h"
#include "gf2d_particles.h"

#define EntityMaxSounds 8
typedef enum {
	ED_Up,			/**<entity is facing up*/
	ED_Down,		/**<entity is facing down*/
	ED_Right,		/**<entity is facing right*/
	ED_Left			/**<entity is facing left*/
}EntityDir;

typedef enum {

	ES_Idle,		/**<entity is currently idle*/
	ES_Walk,		/**<entity is currently walking*/
	ES_Attack		/**<entity is currently attacking*/

}EntityState;

typedef struct {
	int hp_max;		/**<entity's max health*/
	int hp_current;	/**<entity's health*/
	int str;		/**<entity's strength*/
	int def;		/**<entity's defence*/
	int agil;		/**<entity's agility*/
}Stats;

typedef struct{

	int xp;			/**<xp of entity*/
	int level;		/**<level of entity*/
	Stats stats;	/**<stat struct for entity*/
	Item equipped_weapon; /**<weapon currently equipped*/
	Item equipped_armor; /**<armor currently equipped*/
	Item equipped_accessory; /**<accessory currently equipped*/
	int selected_item; /**<item the entity is holding/selecting*/
	Item *inventory[MAX_ITEMS]; /**<inventory for the player but could also be used as enemy drop table*/

}RpgElements;

typedef struct Entity_S {

	Uint8 inuse;
	Uint32 id;

	TextLine name;
	EntityState state;
	EntityDir dir;
	Action action;
	Sound *sound[8];

	//Physics
	cpShape *cpshape;
	cpBody *cpbody;

	cpVect spawnpos; /**<Spawn position of entity*/
	Shape shape;
	Vector2D velocity;
	Vector2D position;

	//Graphics
	Actor actor;
	Vector3D rotation;
	Vector2D scale;
	Vector2D scalecenter;
	Vector2D flip;
	Sprite *sprite;

	ParticleEmitter *pe;

	void(*update)(struct Entity_S *self);	/**<update entity per frame*/
	void(*think)(struct Entity_S *self);	/**<think before update*/
	void(*draw)(struct Entity_S *self);		/**<render entity*/
	int(*touch)(struct Entity_S *self);		/**<currently not using*/
	int(*cpTouch)(cpBody *self, cpBody *other); /**<called when player collides something*/
	int(*damage)(struct Entity_S *attacker, struct Entity_S *inflicted);	/**<when this entity takes damage*/
	void(*free)(struct Entity_S *self);		/**<free entity from game*/

	/*game specific data*/
	TextLine    targetLevel;
	TextLine    targetEntity;
	Uint32      targetId;

	
	RpgElements rpg;	/**<all the rpg elements that an entity will need*/

	float attack_rate;	/**<rate of which an entity can attack*/
	float cooldown;		/**<cooldown for attacks*/
	float iframes;		/**<time of invicibility frames*/

	//Game data
	void *data;			/**<data pointer*/

} Entity;

/**
* @brief intitalizes entity system
* @param maxEntites Number of entities that can be in the system
*/
void gf2d_entity_system_init(Uint32 maxEntities);

/**
* @brief creates new entity
*/
Entity *gf2d_entity_new();

/**
* @brief frees entity
* @param self Entity to be freed
*/
void gf2d_entity_free(Entity *self);

/**
* @brief safely frees chipmunk physics
* @param Entity to be freed
*/
void gf2d_entity_free_physics(Entity *self);

/**
* @brief free all entities
*/
void gf2d_entity_free_all();

/**
* @brief draw entity
*/
void gf2d_entity_draw(Entity *self);

/**
* @brief draw all entities
*/
void gf2d_entity_draw_all();

/**
* @brief Draws the shape of an entity
* @param The entity shape to be drawn
*/
void gf2d_entity_draw_shape(Entity *self);

/**
* @brief Draws the shape of all entities
*/
void gf2d_entity_draw_shape_all();

void gf2d_entity_think(Entity *self);

void gf2d_entity_think_all();

void gf2d_entity_update(Entity *self);

void gf2d_entity_update_all();

void gf2d_entity_pre_sync_all();

void gf2d_entity_post_sync_all();

Entity *gf2d_entity_get_by_id(Uint32 id);

Entity *gf2d_entity_get_by_name(const char *name);

Entity *gf2d_entity_get_by_name_id(const char *name, Uint32 id);

Entity *gf2d_entity_iterate(Entity *start);

int num_entities();

void sort_entities();

int comparator(const void *p, const void *q);

#endif

