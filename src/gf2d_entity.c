#include <stdlib.h>
#include "simple_logger.h"
#include "gf2d_sprite.h"
#include "gf2d_entity.h"
#include "Player.h"

typedef struct {

	Entity *entityList;
	Uint32 maxEntities;
	Uint64 autoincrement;

}EntityManager;

static EntityManager entityManager = { 0 };

void gf2d_entity_system_close()
{
	int i;
	if (entityManager.entityList != NULL)
	{
		for (i = 0; i < entityManager.maxEntities; i++)
		{
			gf2d_entity_free(&entityManager.entityList[i]);
		}
		free(entityManager.entityList);
	}
	memset(&entityManager, 0, sizeof(EntityManager));
	slog("entity system closed");
}

void gf2d_entity_system_init(Uint32 maxEntities) {
	if (!maxEntities) {
		slog("Cannot allocate zero entities.");
		return;
	}
	entityManager.entityList = (Entity*)malloc(sizeof(Entity)*maxEntities);
	if (!entityManager.entityList) {
		slog("failed to allocate new entity");
		gf2d_entity_system_close();
		return;
	}
	memset(entityManager.entityList, 0, sizeof(Entity)*maxEntities);
	entityManager.maxEntities = maxEntities;
	atexit(gf2d_entity_system_close);
	slog("entity system initialized");

}

int gf2d_entity_validate_entity_pointer(void *p)
{
	Entity *ent;
	if (!p)return 0;
	ent = (Entity *)p;
	if (ent < entityManager.entityList)return 0;
	if (ent >= &entityManager.entityList[entityManager.maxEntities])return 0;
	return 1;
}

Entity *gf2d_entity_iterate(Entity *start)
{
	Entity *p = NULL;
	if (!start)p = entityManager.entityList;
	else
	{
		p = start;
		p++;
	}
	for (; p != &entityManager.entityList[entityManager.maxEntities]; p++)
	{
		if (p->inuse)return p;
	}
	return NULL;
}

Entity *gf2d_entity_new()
{
	int i;
	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inuse)continue;
		entityManager.entityList[i].inuse = 1;
		entityManager.entityList[i].scale.x = 1;
		entityManager.entityList[i].scale.y = 1;
		return &entityManager.entityList[i];
	}
	slog("all entities in use, cannot provide a new entity");
	return NULL;
}

void gf2d_entity_free(Entity *self)
{
	if (!self)return;
	if (self->sprite != NULL)
	{
		gf2d_sprite_free(self->sprite);
	}

	if(!&self->actor)
		gf2d_actor_free(&self->actor);

	if(self->cpbody != NULL)
		gf2d_entity_free_physics(self);
	//Why does this have to be Entity *???
	
	/*memset(self, 0, sizeof(Entity));
	if (self == player_get())
		self = NULL;*/
	memset(self, 0, sizeof(Entity));
}

void gf2d_entity_free_physics(Entity *self) {
	if (!self)return;
	cpSpaceRemoveShape(self->cpbody->space, self->cpshape);
	cpSpaceRemoveBody(self->cpbody->space, self->cpbody);
	cpShapeFree(self->cpshape);
	cpBodyFree(self->cpbody);
}

void gf2d_entity_free_all() {
	int i;

	for (i = 0; i < entityManager.maxEntities; i++) {
		
		if (entityManager.entityList[i].sprite != NULL)
		{
			gf2d_sprite_free(entityManager.entityList[i].sprite);
		}
		memset(&entityManager.entityList[i], 0, sizeof(Entity));
	}
}

void gf2d_entity_draw(Entity * self)
{

	if (!self)return;
	if (!self->inuse)return;
	Vector2D cam = camera_get_position();
	gf2d_sprite_draw(
		self->actor.sprite,
		vector2d(self->position.x - cam.x, self->position.y - cam.y),
		//self->position,
		&self->scale,
		&self->scalecenter,
		&self->rotation,
		&self->flip,
		NULL,
		(Uint64)self->actor.frame);

	if (self->draw != NULL) {
		self->draw(self);
	}

}

void gf2d_entity_draw_all() {
	int i;

	for (i = 0; i < entityManager.maxEntities; i++) {
		//if (entityManager.entityList[i]._inuse == 0)continue;
		gf2d_entity_draw(&entityManager.entityList[i]);
	}
}

void gf2d_entity_draw_shape(Entity *self) {

	if (!self)return;
	if (!self->inuse)return;
	Vector2D cam = camera_get_position();

	gf2d_shape_draw(self->shape, gf2d_color(1, 1, 0, 1), vector2d(self->position.x - cam.x, self->position.y - cam.y));

}

void gf2d_entity_draw_shape_all() {
	int i;

	for (i = 0; i < entityManager.maxEntities; i++) {
		//if (entityManager.entityList[i]._inuse == 0)continue;
		gf2d_entity_draw_shape(&entityManager.entityList[i]);
	}
}

void gf2d_entity_think(Entity *self) {

	if (!self)return;
	if (!self->inuse)return;

	if (self->think != NULL) {
		self->think(self);
	}
}

void gf2d_entity_think_all() {

	int i;

	for (i = 0; i < entityManager.maxEntities; i++) {
		gf2d_entity_think(&entityManager.entityList[i]);
	}

}

void gf2d_entity_update(Entity *self) {

	if (!self)return;
	if (!self->inuse)return;

	if (self->update != NULL) {
		self->update(self);
	}
}

void gf2d_entity_update_all() {
	int i;

	for (i = 0; i < entityManager.maxEntities; i++) {
		gf2d_entity_update(&entityManager.entityList[i]);
	}
}

void gf2d_entity_pre_sync_body(Entity *self)
{
	if (!self)return;// nothin to do
	vector2d_copy(self->body.velocity, self->velocity);
	vector2d_copy(self->body.position, self->position);
}

void gf2d_entity_post_sync_body(Entity *self)
{
	if (!self)return;// nothin to do
//    slog("entity %li : %s old position(%f,%f) => new position (%f,%f)",self->id,self->name,self->position,self->body.position);
	vector2d_copy(self->position, self->body.position);
	vector2d_copy(self->velocity, self->body.velocity);
	
}

void gf2d_entity_pre_sync_all()
{
	int i;
	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inuse == 0)continue;
		gf2d_entity_pre_sync_body(&entityManager.entityList[i]);
	}
}

void gf2d_entity_post_sync_all()
{
	int i;
	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inuse == 0)continue;
		gf2d_entity_post_sync_body(&entityManager.entityList[i]);
	}
}


Entity *gf2d_entity_get_by_id(Uint32 id)
{
	int i;
	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inuse == 0)continue;
		if (entityManager.entityList[i].id == id)
		{
			return &entityManager.entityList[i];
		}
	}
	return NULL;
}

Entity *gf2d_entity_get_by_name_id(const char *name, Uint32 id)
{
	int i;
	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inuse == 0)continue;
		if ((gf2d_line_cmp(entityManager.entityList[i].name, name) == 0) && (entityManager.entityList[i].id == id))
		{
			return &entityManager.entityList[i];
		}
	}
	return NULL;
}

void name_all_entity() {

	int i;
	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].name) {
			slog("Name is: %s", entityManager.entityList[i].name);
		}
	}

}