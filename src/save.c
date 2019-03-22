#include "save.h"
#include "simple_logger.h"
#include "gf2d_text.h"


void save_write_out(SaveInfo save, char *outname) {
	
	int i;
	FILE *outfile;

	//TODO:Create save directory for save file

	if (!outname) {
		
		slog("no file to write out to");
		return;
	}

	outfile = fopen(outname, "wb");
	if (!outfile) {
		slog("Failed to open file for saving");
		return;
	}

	//snprintf(line, sizeof(line), "Number: %i", i + 1);
	fwrite(&save, sizeof(SaveInfo), 1, outfile);
	
	fclose(outfile);
}

void save_load_in(char *filename) {
	int i;
	FILE *infile;
	SaveInfo saveinfo;

	Entity *player;
	infile = fopen(filename, "rb");
	slog("Trying to read file: %s", filename);
	if (!infile) {
		slog("Failed to open file for loading");
		return;
	}
	fread(&saveinfo, sizeof(SaveInfo), 1, infile);
	slog("saveinfo hp: %i", saveinfo.player.rpg.stats.hp_current);
	player = player_get();
	
	player->rpg = saveinfo.player.rpg;

	fclose(infile);

}