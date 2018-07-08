#include "level.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "actor.h"
#include "genmap.h"
#include "mapgen.h"

level_t* init_level(int width,
                    int height) {
	level_t * level = (level_t*) malloc(sizeof(level_t));

	map_t * _map = create_map(height, width);
	_map = mapgen_shrew(_map);
	_map = mapgen_rooms_shrew(_map);
	_map = make_walls_shrew(_map);
	level->map = _map;
	level->map->rooms = NULL;

	return level;
}

void draw_level(level_t* level,box_t box, actor_t * actor,
	 	feature_t * features,	msgs_t * msgs) {
	render(level->map, actor, features, msgs, box);
	refresh();
}


void free_level(level_t* level) {
	free_map(level->map);
}
