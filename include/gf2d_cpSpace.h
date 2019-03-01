#include <chipmunk.h>
#include <chipmunk_types.h>

//Type of collisions throughout the game 
#define	PLAYER_TYPE 1 /**Player themself*/
#define MONSTER_TYPE 2 /**Enemies*/
#define INTERACTABLE_TYPE 4 /**Entities such as chests*/
#define TRANSITION_TYPE 8 /**Level transitions*/
#define STATIC_TYPE 16 /**Statics*/

/**
* @brief Initializes chipmunk space
* @param Chipmunk space
*/
cpSpace *gf2d_cpSpace_init(void);

cpBool player_touch_ground(cpArbiter *arb, cpSpace *space, void *data);

cpBool player_touch_monster(cpArbiter *arb, cpSpace *space, void *data);