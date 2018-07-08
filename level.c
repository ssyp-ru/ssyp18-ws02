#include "level.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "actor.h"
#include "mapgen.h"

level_t* init_level(int width,
                    int height) {
	level_t * level = (level_t*) malloc(sizeof(level_t));

	level->map = generate_map(width, height);
	return level;
}

void draw_level(level_t* level) {
	draw_map(level->map);

	refresh();
}


void free_level(level_t* level) {
	free_map(level->map);
}
