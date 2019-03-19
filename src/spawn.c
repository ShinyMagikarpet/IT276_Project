#include "spawn.h"
#include "level.h"
#include "player.h"
#include "Bug.h"
#include "bug_hive.h"
#include "simple_logger.h"
#include "house.h"

static Spawn spawnlist[] =
{
	{
		"player_start",
		player_spawn
	},
	{
		"bug_start",
		bug_spawn
	},
	{
		"bug_hive_start",
		bug_hive_spawn
	},
	{
		"house",
		house_spawn
	},
	{0}
};

void spawn_entity(const char *name, cpVect position, Uint32 id, SJson *args, cpSpace *space)
{
	Spawn *spawn;
	Entity *ent;
	if (!name)
	{
		slog("no spawn name provided");
		return;
	}
	for (spawn = spawnlist; spawn->name != 0; spawn++)
	{
		if (strcmp(spawn->name, name) == 0)
		{
			if (spawn->spawn)
			{
				ent = spawn->spawn(position, args, space);
				if (ent)
				{
					ent->id = id;
				}
				return;
			}
		}
	}
	slog("failed to spawn entity %s", name);
}

/*eol@eof*/
