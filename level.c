#include "level.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "actor.h"
#include "mapgen.h"

level_t* initLevel(int amountOfEntities, int width,
                   int height) {
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);

	time_t t;
	srand((unsigned)time(&t));

	level_t* level = (level_t*)malloc(sizeof(level_t));
	level->map = generateMap(width, height);
	level->actors = initActors(level, amountOfEntities);

	return level;
}

void drawLevel(level_t* level) {
	drawMap(level->map);
	drawActors(level->actors);

	refresh();
}

bool updateLevel(level_t* level) {
	drawLevel(level);
	return updateActors(level->actors);
}

void freeLevel(level_t* level) {
	freeMap(level->map);
	freeActors(level->actors);
	free(level);
}
