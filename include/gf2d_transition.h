#ifndef __GF2D_TRANSITION_H__
#define __GF2D_TRANSITION_H__
#include "gf2d_text.h"
#include "gf2d_list.h"

/**
  * @brief TransitionData structure will hold the necessary data for level transition
*/
typedef struct Transition{
	TextLine	targetLevel;	/**<Target level name*/
	int			value;				/**<Value of the transitioner*/
	Uint32		inuse;			/**<Check if currently in use*/
}Transition;

/**
  * @brief Initialize the transition system
  * @param maximum levels needed
*/
void gf2d_transition_system_init(Uint32 maxEntities);

/**
  * @brief create a new transition level
  * @return address of new transition
*/
Transition *gf2d_transition_new();

/**
  * @brief free the transition from the list
  * @param transition to be freed
*/
void gf2d_transition_free(Transition *self);

/**
  * @brief Get the address of transition by value
  * @param maximum levels needed
*/
Transition *gf2d_transition_get_by_value(int value);

void name_all_transitions();


#endif // !__GF2D_TRANSITION_H__
