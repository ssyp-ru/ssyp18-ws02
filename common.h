#pragma once

#include <ncurses.h>
#define FLAG_SOLID 0x1
#define FLAG_TRANS 0x2
#define FLAG_DESTR 0x4

typedef enum feature_type {
	NOTHING, DROPPED_ITEM, CONTAINER,
	DOOR, DEAD_BADGER, STAIR
} ftype_t;

typedef struct tile{
	chtype symbol;
  int flags;
} tile_t;

typedef struct feature{
	struct inventory * inventory;
	ftype_t type;
	int x, y;
	int flags;
	char * description;
	chtype symbol;
} feature_t;

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


typedef struct actors_vect {
	struct actor* all_actors;
	unsigned int len;
	unsigned int capacity;
} avect_t;

typedef struct room {
	int x, y;
	int height, width;
} room_t;


typedef struct room_vector {
	room_t* data;
	size_t length;
	size_t capacity;
} room_vector_t;


typedef struct map {
	tile_t* buffer;
	room_vector_t * rooms;
	int height, width;
} map_t;

typedef struct level {
	map_t* map;
	avect_t* actors;
} level_t;

typedef struct kdtree {
	struct kdtree * rbranch;
	struct kdtree * lbranch;
	feature_t * node;
} kdtree_t;

typedef struct box{
	int x,y;
	int width, height;
} box_t;

typedef struct features_vec {
	feature_t ** buffer;
	int last;
	size_t capacity;
} fvec_t;
