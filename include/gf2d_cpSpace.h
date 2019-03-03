#include <chipmunk.h>
#include <chipmunk_types.h>

//Types of collisions throughout the game 
#define	PLAYER_TYPE 1		/**Player themself*/
#define MONSTER_TYPE 2		/**Enemies*/
#define INTERACTABLE_TYPE 4 /**Entities such as chests*/
#define TRANSITION_TYPE 8	/**Level transitions*/
#define STATIC_TYPE 16		/**Statics*/
#define WEAPON_TYPE 32		/**Weapons*/


#define SPACE_STEP 1.0 / 60.0 /**Steps for space*/

/**
* @brief Initializes chipmunk space
* @param Chipmunk space
*/
cpSpace *gf2d_cpSpace_init(void);

/**
* @brief Updates space
* @param Chipmunk space
*/
void *gf2d_cpSpace_update(cpSpace *space);

/**
* @brief Function to be called when player hits type static
*/
cpBool touch_ground(cpArbiter *arb, cpSpace *space, void *data);

/**
* @brief Function to be called if player collides shape type monster at first frame
*/
cpBool player_touch_monster_begin(cpArbiter *arb, cpSpace *space, void *data);

/**
* @brief Function to be called if player collides shape type monster continously
*/
cpBool player_touch_monster_presolve(cpArbiter *arb, cpSpace *space, void *data);

/**
* @brief Function to be called after player and monster end collision
*/
void player_touch_monster_separate(cpArbiter *arb, cpSpace *space, void *data);


/**
* @brief Function to be called after monster touches monster
*/
cpBool monster_touch_monster_begin(cpArbiter *arb, cpSpace *space, void *data);
