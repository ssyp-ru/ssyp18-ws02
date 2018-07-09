#include "level.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "actor.h"
#include "genmap.h"
level_t* init_level(int width,
                    int height) {
	level_t * level = calloc(1, sizeof(level_t));

	map_t * _map = create_map(height, width);
	/*_map = mapgen_shrew(_map);
	_map = mapgen_rooms_shrew(_map);
	_map = make_walls_shrew(_map);*/
	_map = mapgen_recdev(_map);
	level->map = _map;
	level->map->rooms = NULL;

	return level;
}

void free_level(level_t* level) {
	free_map(level->map);
	free(level);
}
