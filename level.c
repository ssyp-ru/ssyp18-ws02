#include "level.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "actor.h"
#include "mapgen.h"

level_t* init_level(int amount_of_entities, int width,
                   int height) {
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);

	time_t t;
	srand((unsigned)time(&t));

	level_t* level = (level_t*)malloc(sizeof(level_t));
	level->map = generate_map(width, height);
	level->actors = init_actors(level, amount_of_entities);

	return level;
}

void draw_level(level_t* level) {
	draw_map(level->map);
	draw_actors(level->actors);

	refresh();
}

bool update_level(level_t* level) {
	draw_level(level);
	return update_actors(level->actors);
}

void free_level(level_t* level) {
	free_map(level->map);
	free_actors(level->actors);
	free(level);
}
