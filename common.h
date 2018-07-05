#pragma once
#include <ncurses.h>

// Defines whether the tile is a wall
#define FLAG_SOLID 0x1
// Defines whether the tile is translucent
#define FLAG_TRANS 0x2
// Defines whether the tile is destructible
#define FLAG_DESTR 0x4

// FIXME: Separate tile flags from actor flags
#define FLAG_ISPLAYER 0x1
#define FLAG_DEAD 0x2

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
	int height, width;
} map_t;

