#pragma once
#include <ncurses.h>

#define FLAG_SOLID 0x1
#define FLAG_TRANS 0x2
#define FLAG_DESTR 0x4

typedef enum feature_type {
	NOTHING, DROPPED_ITEM, CONTAINER,
	DOOR, DEAD_BADGER
} ftype_t;

typedef struct tile{
	chtype symbol;
  int flags;
} tile_t;

typedef struct map{
	tile_t * buffer;
	int height;
 	int width;
} map_t;	

typedef struct feature{
	inventory_t * inventory;
	ftype_t type;
	int flags;
	char * description;
	chtype symbol;
} feature_t;

typedef struct level_t {
	map_t * map;
	kdtree_t * features;
} level_t;

// Function for picking up items from a feature
void pickUp(actor_t * actor, feature_t feature);

//Function for dropping item on a floor
void throwAway(item_t * item, level_t * level, int xPos, int yPos);

#define UNPACK(varname, map_ptr) tile_t (*varname)[(map_ptr)->width] = (tile_t (*)[(map_ptr)->width]) map_ptr->buffer 

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

