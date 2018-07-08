#include <stdlib.h>
#include "genmap.h"
#include "common.h"

map_t * create_map(int height,int width) {
	map_t *map  = calloc(1, sizeof(map_t));
	map->buffer = calloc(width*height, sizeof(tile_t));
	map->height = height;
	map->width  = width;
	map->rooms  = NULL;
	return map;
}

void free_map(map_t * map) {
	if(map->rooms)
		vector_free(map->rooms);
	free(map->buffer);
	free(map);
}

void print_map(map_t * _map) {
	UNPACK(land, _map); // Defines 2D-array of tile_t named map
	for(int i = 0; i < _map->height; i++) {
		for(int j = 0; j < _map->width; j++)
			mvaddch(i, j, land[i][j].symbol);
	}
	refresh();
}
