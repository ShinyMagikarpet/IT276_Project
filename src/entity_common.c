#include "gf2d_entity.h"
#include "camera.h"
#include "simple_logger.h"
#include "entity_common.h"
#include "level.h"
#include "player.h"

Collision entity_scan_hit(Entity *self, Vector2D start, Vector2D end)
{
	Shape s = { 0 };
	Collision c;
	ClipFilter f = {
		PLAYER_LAYER | MONSTER_LAYER,          /**<layer mask to clip against*/
		self->body.team,           /**<ignore any team ==*/
		&self->body
	};
	s = gf2d_shape_edge(start.x, start.y, end.x, end.y);
	gf2d_space_body_collision_test_filter(level_get_space(), s, &c, f);
	gf2d_shape_draw(s, gf2d_color(255, 255, 0, 255), camera_get_offset());
	return c;
}

Collision entity_block_hit(Entity *self, Rect box)
{
	Shape s = { 0 };
	Collision c;
	ClipFilter f = {
		PLAYER_LAYER | MONSTER_LAYER,          /**<layer mask to clip against*/
		self->body.team,           /**<ignore any team ==*/
		&self->body
	};
	s = gf2d_shape_from_rect(box);
	gf2d_space_body_collision_test_filter(level_get_space(), s, &c, f);
	gf2d_shape_draw(s, gf2d_color(255, 255, 0, 255), camera_get_offset());
	return c;
}

void entity_damage(Entity *target, Entity *killer, int damage, float kick)
{
	if ((!target) || (!killer))
	{
		slog("missing entity data");
		return;
	}
	if (target->damage != NULL)
	{
		target->damage(target, damage, killer);
		entity_push(killer, target, kick);
	}
}

void entity_push(Entity *self, Entity *other, float amount)
{
	Vector2D push;
	if ((!self) || (!other))
	{
		slog("missing an entity");
		return;
	}
	vector2d_sub(push, other->position, self->position);
	vector2d_set_magnitude(&push, amount);
	vector2d_add(other->velocity, other->velocity, push);
	vector2d_add(other->body.velocity, other->body.velocity, push);
}

int entity_camera_view(Entity *self)
{
	Rect r, c;
	if (!self)return 0;
	c = camera_get_dimensions();
	r = gf2d_shape_get_bounds(self->shape);
	vector2d_add(r, r, self->position);
	return gf2d_rect_overlap(r, c);
}

int entity_roof_check(Entity *self, float width)
{
	Rect r;
	Collision c;
	if (!self)return 0;
	r = gf2d_shape_get_bounds(self->shape);
	r.x += 0.1;
	r.w -= 0.2;
	r.y -= (0.1 + width);
	r.h = width;
	vector2d_add(r, r, self->position);
	c = gf2d_space_shape_test(level_get_space(), gf2d_shape_from_rect(r));
	if (c.shape != NULL)
	{
		gf2d_shape_draw(*c.shape, gf2d_color(255, 255, 0, 255), camera_get_offset());
	}
	return c.collided;
}


int entity_ground_check(Entity *self, float width)
{
	Rect r;
	Collision c;
	if (!self)return 0;
	r = gf2d_shape_get_bounds(self->shape);
	r.x += 0.1;
	r.w -= 0.2;
	r.y += r.h + 0.1;
	r.h = width;
	vector2d_add(r, r, self->position);
	c = gf2d_space_shape_test(level_get_space(), gf2d_shape_from_rect(r));
	if (c.shape != NULL)
	{
		gf2d_shape_draw(*c.shape, gf2d_color(0, 255, 255, 255), camera_get_offset());
	}
	return c.collided;
}

int entity_left_check(Entity *self, float width)
{
	Rect r;
	Collision c;
	if (!self)return 0;
	r = gf2d_shape_get_bounds(self->shape);
	r.x -= (0.1 + width);
	r.w = width;
	r.y += 0.1;
	r.h -= 0.2;
	vector2d_add(r, r, self->position);
	c = gf2d_space_shape_test(level_get_space(), gf2d_shape_from_rect(r));
	if (c.shape != NULL)
	{
		gf2d_shape_draw(*c.shape, gf2d_color(255, 0, 255, 255), camera_get_offset());
	}
	return c.collided;
}

int entity_right_check(Entity *self, float width)
{
	Rect r;
	Collision c;
	if (!self)return 0;
	r = gf2d_shape_get_bounds(self->shape);
	r.x += r.w + 0.11;
	r.w = width;
	r.y += 0.1;
	r.h -= 0.2;
	vector2d_add(r, r, self->position);
	c = gf2d_space_shape_test(level_get_space(), gf2d_shape_from_rect(r));
	if (c.shape != NULL)
	{
		gf2d_shape_draw(*c.shape, gf2d_color(255, 0, 0, 255), camera_get_offset());
	}
	return c.collided;
}

void entity_world_snap(Entity *self)
{
	if (entity_ground_check(self, 0.1))
	{
		self->position.y -= 0.1;
	}
	if (entity_roof_check(self, 0.1))
	{
		self->position.y += 0.1;
	}
	if (entity_right_check(self, 0.1))
	{
		self->position.x -= 0.1;
	}
	if (entity_left_check(self, 0.1))
	{
		self->position.x += 0.1;
	}
}

void entity_clear_all_but_player()
{
	Entity *ent = NULL;
	for (ent = gf2d_entity_iterate(NULL); ent != NULL; ent = gf2d_entity_iterate(ent))
	{
		if (ent == player_get())continue;
		gf2d_entity_free(ent);
	}
}

Entity *entity_get_from_body(Body *body)
{
	if (!body) return NULL;
	if (!gf2d_entity_validate_entity_pointer(body->data))return NULL;
	return (Entity*)body->data;
}

/*eol@eof*/
