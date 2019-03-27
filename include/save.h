#include <stdio.h>
#include "gf2d_entity.h"
#include "gf2d_transition.h"

typedef struct SaveInfo_S{
	Entity player; /**< Player entity to save */
	Transition level; /**< level in which to load*/
}SaveInfo;

/**
* @brief write out a save file
* @param save The save info to write out
* @param outname Name of file to write out
*/
void save_write_out(SaveInfo save, char *outname);

/**
* @brief load in save file
* @param filename Name of the file to load in
*/
void save_load_in(char *filename);
