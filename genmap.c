#include <stdlib.h>
#include "genmap.h"
#include "roomvector.h"
#include "common.h"

map_t * create_map(int height, int width) {
	map_t *map = calloc(1, sizeof(map_t));
	map->rooms = NULL;
	map->buffer = calloc(width*height, sizeof(tile_t));
	map->height = height;
	map->width = width;
	return map;
}

void free_map(map_t * map) {
	if(map->rooms)
		rooms_vector_free(map->rooms);
	free(map->buffer);
	free(map);
}
