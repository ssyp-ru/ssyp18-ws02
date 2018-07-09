#include <stdlib.h>
#include "genmap.h"
#include "common.h"

map_t * create_map(int height, int width) {
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

