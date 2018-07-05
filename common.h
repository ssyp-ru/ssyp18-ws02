#pragma once

#define FLAG_PERM  0x1
#define FLAG_TRANS 0x2
#define FLAG_DESTR 0x4

#define FLAG_ISPLAYER 0x1
#define FLAG_DEAD 0x2

#include <ncurses.h>
#include "roomvector.h"

/**
 * This macros defines a new variable which allows to access map_t.buffer
 * as a two-dimensional array with width of map_t.width.
 * 
 * Example:
 * void map_foo(map_t* map_, int x, int y) {
 * 		UNPACK(map, map_);
 * 		if(map[x][y] == '@') {
 * 			...
 * 		}
 * }
*/

#define UNPACK(varname, map_ptr) tile_t (*varname)[(map_ptr)->width] = (tile_t (*)[(map_ptr)->width]) map_ptr->buffer 


typedef struct tile{
	chtype symbol;
  int flags;
} tile_t;


typedef struct map{
	tile_t* buffer;
	roomVector_t* rooms;
	int amountOfRooms;
	int height, width;
} map_t;


typedef struct actor{
	chtype symbol;
	int flags;
	int x, y;
} actor_t;


typedef struct actors{
	actor_t* array;
	int amount;
} actors_t;


typedef struct level{
	map_t* map;
	actors_t* actors;
} level_t;




