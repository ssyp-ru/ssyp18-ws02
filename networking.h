#pragma once
#include "common.h"
#include "net.h"

typedef struct {
	int x;
	int y;
	int width;
	int height;
	tile_t s[MAX_RECV / 8 - 2];
} update_map_t;
// Remember that sizeof(update_map_t) would be greater than 
// the actual information length in it due to tile_t array size.
// Please consider that and call mesg_serialize() with size
// (sizeof(tile_t)*width*height + sizeof(int)*4)

// update_###_t

void map_update(map_t* _map, update_map_t* msg);
update_map_t* map_serialize(map_t* _map, int y, int x, int height, int width);

typedef struct {
	actor_t base;
	int inv_count;
	int ids[MAX_RECV / 8];
} update_actor_t;
