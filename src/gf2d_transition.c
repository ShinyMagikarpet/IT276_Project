#include "gf2d_transition.h"
#include "simple_logger.h"

typedef struct {

	Transition		*levelList;
	Uint32			maxLevels;
	Uint32			autoincrement;

}TransitionManager;

static TransitionManager transitionManager = { 0 };

void gf2d_transition_system_close()
{
	int i;
	if (transitionManager.levelList != NULL)
	{
		for (i = 0; i < transitionManager.maxLevels; i++)
		{
			gf2d_transition_free(&transitionManager.levelList[i]);
		}
		free(transitionManager.levelList);
	}
	memset(&transitionManager, 0, sizeof(TransitionManager));
	slog("transition system closed");
}

void gf2d_transition_system_init(Uint32 maxLevels) {
	if (!maxLevels) {
		slog("Cannot allocate zero levels.");
		return;
	}
	transitionManager.levelList = (Transition*)malloc(sizeof(Transition)*maxLevels);
	if (!transitionManager.levelList) {
		slog("failed to allocate new level");
		gf2d_entity_system_close();
		return;
	}
	memset(transitionManager.levelList, 0, sizeof(Transition)*maxLevels);
	transitionManager.maxLevels = maxLevels;
	atexit(gf2d_transition_system_close);
	slog("transition system initialized");

}

void gf2d_transition_free(Transition *self)
{
	if (!self)return;
	memset(self, 0, sizeof(Transition));
}

Transition *gf2d_transition_new() {

	int i;
	for (i = 0; i < transitionManager.maxLevels; i++)
	{
		if (transitionManager.levelList[i].inuse)continue;
		transitionManager.levelList[i].inuse = 1;
		//slog("added new transition");
		return &transitionManager.levelList[i];
	}
	slog("all levels in use, cannot provide a new level");
	return NULL;

}

Transition *gf2d_transition_get_by_value(int value) {
	int i;
	for (i = 0; i < transitionManager.maxLevels; i++) {
		if (transitionManager.levelList[i].value != value)continue;
		return &transitionManager.levelList[i];
	}
	return NULL;
}

Transition *gf2d_check_if_duplicate(Transition *self) {
	int i, count, original;

	original = -1;
	count = 0;
	for (i = 0; i < transitionManager.maxLevels; i++) {
		if (strcmp(transitionManager.levelList[i].targetLevel, self->targetLevel) == 0 && 
			transitionManager.levelList[i].value == self->value) {
			if (original == -1)original = i;
			count++;
			if (count >= 2) {
				gf2d_transition_free(&transitionManager.levelList[i]);
				//slog("transition manage: %s, %i", transitionManager.levelList[original].targetLevel, transitionManager.levelList[original].value);
				return &transitionManager.levelList[original];
			}
			//slog("count: %i", count);
		}
			
	}
	return self;
}

void name_all_transitions() {
	int i;
	for (i = 0; i < transitionManager.maxLevels; i++) {
		slog("transition level: %s", transitionManager.levelList[i].targetLevel);
	}
}