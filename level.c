#include "actors.h"
#include "mapgen.h"
#include "common.h"
#include <time.h>
#include <stdlib.h>


level_t* initLevel(int amountOfEntities, int width, int height)
{
	start_color();	
	init_pair(1, COLOR_WHITE, COLOR_BLACK);

	time_t t;
	srand((unsigned) time(&t));

	level_t* level = (level_t*) malloc(sizeof(level_t));
	level->map = generateMap(width, height);
	level->actors = initActors(level->map, amountOfEntities);

	return level;
}


void drawLevel(level_t* level)
{
	drawMap(level->map);
	drawActors(level->actors);

	refresh();		
}


bool updateLevel(level_t* level)
{
	drawLevel(level);
	return behaveActors(level->map, level->actors);
}


void freeLevel(level_t* level)
{
	freeMap(level->map);
	freeActors(level->actors);
	free(level);
}
