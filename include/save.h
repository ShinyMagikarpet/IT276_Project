#include <stdio.h>
#include "gf2d_entity.h"
#include "gf2d_transition.h"

typedef struct SaveInfo_S{
	Entity player;
	Transition level;
}SaveInfo;

struct threeNum
{
	int n1, n2, n3;
};

void save_write_out(SaveInfo save, char *outname);

void save_load_in(char *filename);
