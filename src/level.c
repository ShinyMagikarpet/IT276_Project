#include "level.h"
#include "camera.h"
#include "spawn.h"
#include "player.h"
#include "gf2d_entity_common.h"
#include "simple_json.h"
#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "gf2d_config.h"
#include <stdio.h>
#include <SDL_mixer.h>
#include "gf2d_windows.h"
#include "gf2d_transition.h"

typedef struct
{
	cpSpace    *space;
	Sprite     *backgroundImage;
	Sprite     *tileLayer;
	Sprite     *tileSet;
	Mix_Music  *backgroundMusic;
}Level;



static Level gamelevel = { 0 };

static cpBody *map_body = NULL;

int *level_alloc_tilemap(int w, int h);

void level_clear()
{
	//gf2d_space_free(gamelevel.space);
	//cpSpaceFree(gamelevel.space);
	gf2d_transition_free_all();
	gf2d_sprite_free(gamelevel.backgroundImage);
	gf2d_sprite_free(gamelevel.tileSet);
	gf2d_sprite_free(gamelevel.tileLayer);
	//gf2d_sprite_clear_all();
	gf2d_sound_clear_all();
	
	
	if (gamelevel.backgroundMusic)
	{
		Mix_FreeMusic(gamelevel.backgroundMusic);
	}
	memset(&gamelevel, 0, sizeof(Level));
	slog("level cleared");
}

void level_info_free(LevelInfo *linfo)
{
	if (!linfo)return;
	if (linfo->tileMap != NULL)
	{
		free(linfo->tileMap);
	}
	sj_free(linfo->spawnList);
	sj_free(linfo->transitionList);
	free(linfo);
}

LevelInfo *level_info_new()
{
	LevelInfo *linfo = NULL;
	linfo = (LevelInfo*)malloc(sizeof(LevelInfo));
	if (!linfo)
	{
		slog("failed to allocate level info");
		return NULL;
	}
	memset(linfo, 0, sizeof(LevelInfo));
	return linfo;
}

LevelInfo *level_info_create(
	const char *backgroundImage,
	const char *backgroundMusic,
	const char *tileSet,
	Vector2D    tileSize,
	Vector2D    tileMapSize
)
{
	LevelInfo *linfo = NULL;
	linfo = level_info_new();
	if (!linfo)return NULL;
	gf2d_line_cpy(linfo->backgroundImage, backgroundImage);
	gf2d_line_cpy(linfo->backgroundMusic, backgroundMusic);
	gf2d_line_cpy(linfo->tileSet, tileSet);
	vector2d_copy(linfo->tileSize, tileSize);
	vector2d_copy(linfo->tileMapSize, tileMapSize);
	linfo->tileMap = level_alloc_tilemap(tileMapSize.x, tileMapSize.y);
	return linfo;
}

int *level_alloc_tilemap(int w, int h)
{
	int *tileMap;
	if ((!w) || (!h))
	{
		slog("cannot have a zero width or height for tilemap");
		return NULL;
	}
	tileMap = (int *)malloc(sizeof(int)*w*h);
	if (!tileMap)
	{
		slog("failed to allocate tilemap data");
		return NULL;
	}
	memset(tileMap, 0, sizeof(int)*w*h);
	return tileMap;
}

void level_info_tilemap_load(LevelInfo *linfo, SJson *tilemap, Uint32 w, Uint32 h)
{
	int i, j;
	SJson *row, *value;
	if ((!linfo) || (!tilemap))
	{
		slog("missing level info or map object");
		return;
	}
	linfo->tileMap = level_alloc_tilemap(w, h);
	if (!linfo->tileMap)
	{
		return;
	}
	for (j = 0; j < h; j++)
	{
		row = sj_array_get_nth(tilemap, j);
		for (i = 0; i < w; i++)
		{
			value = sj_array_get_nth(row, i);
			sj_get_integer_value(value, &linfo->tileMap[j*w + i]);
		}
	}
}

LevelInfo *level_info_load(char *filename)
{
	LevelInfo *linfo = NULL;
	SJson *json, *world;
	if (!filename)return NULL;
	json = sj_load(filename);
	if (!json)
	{
		slog("failed to load level file %s", filename);
		return NULL;
	}

	linfo = level_info_new();

	if (!linfo)
	{
		return NULL;
	}

	world = sj_object_get_value(json, "world");

	if (!world)
	{
		slog("missing world object in level file %s", filename);
		sj_free(json);
		level_info_free(linfo);
		return NULL;
	}



	gf2d_line_cpy(linfo->backgroundImage, sj_get_string_value(sj_object_get_value(world, "backgroundImage")));

	//Used to get size of background image for better camera management
	sj_value_as_vector2d(sj_object_get_value(world, "backgroundSize"), &linfo->backgroundSize);
	slog("background size: %f, %f", linfo->backgroundSize.x, linfo->backgroundSize.y);

	gf2d_line_cpy(linfo->backgroundMusic, sj_get_string_value(sj_object_get_value(world, "backgroundMusic")));
	gf2d_line_cpy(linfo->tileSet, sj_get_string_value(sj_object_get_value(world, "tileSet")));

	sj_value_as_vector2d(sj_object_get_value(world, "tileMapSize"), &linfo->tileMapSize);
	slog("loaded tile size of %f,%f", linfo->tileMapSize.x, linfo->tileMapSize.y);

	level_info_tilemap_load(linfo, sj_object_get_value(world, "tileMap"), (Uint32)linfo->tileMapSize.x, (Uint32)linfo->tileMapSize.y);

	sj_value_as_vector2d(sj_object_get_value(world, "tileSize"), &linfo->tileSize);

	//Changed Code to allow frames per line for spritesheets
	sj_get_integer_value(sj_object_get_value(world, "tilesPerLine"), &linfo->framesperline);

	slog("Frames per line: %i", linfo->framesperline);

	linfo->spawnList = sj_copy(sj_object_get_value(world, "spawnList"));
	int count = sj_array_get_count(linfo->spawnList);
	slog("Spawn list count = %i", count);

	//TODO stuff here
	linfo->transitionList = sj_copy(sj_object_get_value(world, "transitionList"));
	count = sj_array_get_count(linfo->transitionList);
	slog("Transition list count = %i", count);

	sj_free(json);
	slog("loaded level info for %s", filename);
	
	return linfo;
}



void level_make_space()
{
	gamelevel.space = gf2d_cpSpace_init();
}

void free_space(cpSpace *space, void *data){
	if (!space)return;
	cpSpaceFree(space);
	space == NULL;
}


void level_make_tile_layer(LevelInfo *linfo)
{
	Sprite *sprite;
	Uint32 clear;
	Uint32 format;
	int i, j;
	if (!linfo)
	{
		slog("no level info provided");
		return;
	}
	if (!gamelevel.tileSet)
	{
		slog("no tileset loaded for level");
		return;
	}
	format = gamelevel.tileSet->surface->format->format;
	sprite = gf2d_sprite_new();
	if (!sprite)return;
	sprite->surface = SDL_CreateRGBSurfaceWithFormat(
		0,
		linfo->tileMapSize.x*linfo->tileSize.x,
		linfo->tileMapSize.y*linfo->tileSize.y,
		linfo->framesperline,
		format);

	clear = SDL_MapRGBA(sprite->surface->format, 0, 0, 0, 0);
	SDL_FillRect(sprite->surface, NULL, clear);

	for (j = 0; j < linfo->tileMapSize.y; j++)
	{
		for (i = 0; i < linfo->tileMapSize.x; i++)
		{
			if (linfo->tileMap[j*(Uint32)linfo->tileMapSize.x + i])
			{
				gf2d_sprite_draw_to_surface(
					gamelevel.tileSet,
					vector2d(i*linfo->tileSize.x, j*linfo->tileSize.y),
					NULL,
					NULL,
					linfo->tileMap[j*(Uint32)linfo->tileMapSize.x + i] - 1,
					sprite->surface);
			}
		}
	}

	sprite->surface = gf2d_graphics_screen_convert(&sprite->surface);

	if (sprite->surface)
	{
		sprite->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), sprite->surface);
		if (!sprite->texture)
		{
			slog("failed to convert tileLayer data to texture");
			gf2d_sprite_free(sprite);
			return;
		}
		SDL_SetTextureBlendMode(sprite->texture, SDL_BLENDMODE_BLEND);
		SDL_UpdateTexture(
			sprite->texture,
			NULL,
			sprite->surface->pixels,
			sprite->surface->pitch);

		sprite->frame_w = sprite->surface->w;
		sprite->frame_h = sprite->surface->h;
		sprite->frames_per_line = 1;

		gf2d_line_cpy(sprite->filepath, "tileLayer");
		gamelevel.tileLayer = sprite;
	}
	else
	{
		slog("failed to convert tile layer to supported format");
		gf2d_sprite_free(sprite);
	}

}



void level_build_tile_space(LevelInfo *linfo)
{

	int i, j, value;
	map_body = cpBodyNewStatic();
	cpBB bb1;// = cpBBNew(128, 32, 160, 0); // l is where left side located, bottom is where bottom is located, etc.
	cpShape *shape;// = cpSpaceAddShape(gamelevel.space, cpBoxShapeNew2(map_body, bb1, 1));;
	for (j = 0; j < linfo->tileMapSize.y; j++)
	{
		for (i = 0; i < linfo->tileMapSize.x; i++)
		{
			if (!linfo->tileMap[j * (Uint32)linfo->tileMapSize.x + i])continue;
			//slog("Tile information: %f, %f, %f, %f", i * linfo->tileSize.x, j * linfo->tileSize.y, linfo->tileSize.x, linfo->tileSize.y);
			bb1 = cpBBNew(i * linfo->tileSize.x, j * linfo->tileSize.y, (linfo->tileSize.x * i) + 32, (linfo->tileSize.y * j) + 32);
			shape = cpBoxShapeNew2(map_body, bb1, 1);
			value = linfo->tileMap[j * (Uint32)linfo->tileMapSize.x + i];
			//slog("value: %i", value);
			//2 is the number for transition tiles where as 1 is plain static
			if (value >= 2) {
				shape->type = TRANSITION_TYPE;
				level_transition_data(linfo->transitionList, value, shape);
				Transition *transitiondata = (Transition *)shape->userData;
				slog("transition data: %s, %i", transitiondata->targetLevel, transitiondata->value);
			}
			cpSpaceAddShape(gamelevel.space, shape);
			//gf2d_space_add_static_shape(gamelevel.space, gf2d_shape_rect(i * linfo->tileSize.x, j * linfo->tileSize.y, linfo->tileSize.x, linfo->tileSize.y));
			//(gamelevel.space, gf2d_shape_rect(i * linfo->tileSize.x, j * linfo->tileSize.y, linfo->tileSize.x, linfo->tileSize.y));
			//TODO: BUILD TILE COLLISION HERE
			
		}
	}
}




void level_update_tile(LevelInfo *linfo, Vector2D position, Uint32 tile)
{
	if (!linfo)
	{
		slog("no tile provided");
		return;
	}
	if (!gf2d_point_in_rect(position, gf2d_rect(0, 0, linfo->tileMapSize.x, linfo->tileMapSize.y)))
	{
		slog("tile position %f,%f is out of bounds of tile map", position.x, position.y);
		return;
	}
	linfo->tileMap[(Uint32)position.y * (Uint32)linfo->tileMapSize.x + (Uint32)position.x] = tile;
}

Vector2D level_position_to_tile(LevelInfo *linfo, Vector2D position)
{
	Vector2D tile = { -1,-1 };
	if (!linfo)
	{
		slog("no level info provided");
		return tile;
	}
	if ((!linfo->tileSize.x) || (!linfo->tileSize.y))
	{
		slog("level info missing tile size data!");
		return tile;
	}
	tile.x = (Uint32)(position.x / linfo->tileSize.x);
	tile.y = (Uint32)(position.y / linfo->tileSize.y);
	return tile;
}

void level_spawn_entities(SJson *spawnList)
{
	int i = 0, count = 0;
	SJson *item;
	cpVect position;
	int id = 0;
	count = sj_array_get_count(spawnList);
	for (i = 0; i < count; i++)
	{
		item = sj_array_get_nth(spawnList, i);
		if (!item)continue;
		sj_value_as_vector2d(sj_object_get_value(item, "position"), &position);
		if (!sj_get_integer_value(sj_object_get_value(item, "name"), &id))
		{
			id = 0;
		}
		spawn_entity(sj_get_string_value(sj_object_get_value(item, "name")), position, id, sj_object_get_value(item, "args"), gamelevel.space);
	}
}

void level_transition_data(SJson *transitionList, int value, cpShape *shape) {
	int i = 0, count = 0;
	SJson *item;
	Transition *data;
	data = gf2d_transition_new();
	if (!data)return;
	count = sj_array_get_count(transitionList);
	for (i = 0; i < count; i++) {
		item = sj_array_get_nth(transitionList, value - 2);
		if (!item)continue;
		sj_get_integer_value(sj_object_get_value(item, "value"), &data->value);
		gf2d_line_cpy(data->targetLevel, sj_get_string_value(sj_object_get_value(item, "targetLevel")));
	}
	data = gf2d_check_if_duplicate(data);
	//slog("data: %s, %i", data->targetLevel, data->value);
	shape->userData = gf2d_transition_get_by_value(data->value);
	
}

void level_init(LevelInfo *linfo, Uint8 space)
{
	if (!linfo)
	{
		return;
	}


	level_make_space();
	gamelevel.backgroundImage = gf2d_sprite_load_image(linfo->backgroundImage);

	gamelevel.tileSet = gf2d_sprite_load_all(
		linfo->tileSet,
		linfo->tileSize.x,
		linfo->tileSize.y,
		linfo->framesperline,
		true);

	gamelevel.backgroundMusic = Mix_LoadMUS(linfo->backgroundMusic);
	if (gamelevel.backgroundMusic)Mix_PlayMusic(gamelevel.backgroundMusic, -1);

	level_make_tile_layer(linfo);

	//slog("background image size X: %f", linfo->backgroundSize.x);
	//Bounds of camera = background/tilemap size
	camera_set_bounds(0, 0, linfo->backgroundSize.x, linfo->backgroundSize.y);

	if (space)
	{
		//level_make_space();
		level_build_tile_space(linfo);
	}
	level_spawn_entities(linfo->spawnList);

	//transition_window_to_normal();
	

	camera_set_dimensions(0, 0, SCREENWIDTH, SCREENHEIGHT);
}

void level_draw()
{
	Vector2D cam;
	Entity *player = player = player_get();
	cam = camera_get_position();
	camera_set_position(cpv(player->cpbody->p.x - (SCREENWIDTH / 2), player->cpbody->p.y - (SCREENHEIGHT / 2)));
	gf2d_sprite_draw_image(gamelevel.backgroundImage, vector2d(-cam.x, -cam.y), vector2d(1,1));
	//gf2d_sprite_draw_image(gamelevel.tileLayer, vector2d(-cam.x, -cam.y), vector2d(2, 2)); //Changed last one to be scale
	gf2d_entity_draw_all();
	gf2d_entity_draw_shape_all();
	if (!player)return;

	//if (gamelevel.space)gf2d_space_draw(gamelevel.space,vector2d(-cam.x,-cam.y));

}

void level_update()
{

	//gf2d_entity_pre_sync_all();

	//gf2d_entity_post_sync_all();

	gf2d_entity_think_all();

	gf2d_entity_update_all();

	gf2d_cpSpace_update(gamelevel.space);
	
}


int body_body_touch(Body *self, Body *other, Collision *collision)
{
	Entity *selfEnt, *otherEnt;
	if ((!self) || (!other))return 0;
	selfEnt = (Entity *)self->data;
	if (!selfEnt)return 0;
	otherEnt = (Entity *)other->data;
	if (!otherEnt)return 0;
	if (selfEnt->touch)
	{
		slog("%s is touching %s", selfEnt->name, otherEnt->name);
		return selfEnt->touch(selfEnt, otherEnt);

	}
	return 0;
}


void level_add_entity(Entity *ent)
{
	if (!ent)return;


	if (!gamelevel.space)

	{
		slog("cannot add entity %s to level, no space defined!", ent->name);
		return;
	}

	if (ent->body.touch == NULL)
	{
		ent->body.touch = body_body_touch;
	}
}

void level_transition(char *filename, const char *playerTarget, Uint32 targetId)
{
	//Entity *target;
	TextLine filepath;
	//TextLine targetname;
	Uint32 id;
	LevelInfo *linfo = NULL;
	level_clear();
	snprintf(filepath, GF2DLINELEN, "levels/%s", filename);
	slog("%s", filepath);
	//gf2d_line_cpy(targetname, playerTarget);
	id = targetId;
	cpBodyDestroy(map_body);
	cpBodyFree(map_body);
	free_space(gamelevel.space, NULL);
	linfo = level_info_load(filepath);
	if (!linfo)return;
	level_init(linfo, 1);

	//target = gf2d_entity_get_by_name_id(targetname, id);
	//if (!target)
	{
	//		slog("expected target %s, %i not found", target, id);
	//		return;
	}
	//player_set_position(cpv(target->position.x, target->position.y - 16));
}

/*eol@eof*/

